#include <algorithm>
#include <cctype>
#include <iostream>
#include <memory>
#include <queue>
#include <stdexcept>
#include <string>
#include <vector>

struct TreeNode {
    int key;
    std::unique_ptr<TreeNode> left, right;
    explicit TreeNode(int value) : key(value) {}
};

class TreeParser {
public:
    explicit TreeParser(std::string input) : text_(std::move(input)) {}

    std::unique_ptr<TreeNode> parse() {
        skipSpace();
        if (position_ == text_.size()) fail("пустая запись");
        auto root = parseNode();
        skipSpace();
        if (position_ != text_.size()) fail("лишние символы после дерева");
        return root;
    }

private:
    std::string text_;
    size_t position_ = 0;

    [[noreturn]] void fail(const std::string& message) const {
        throw std::runtime_error(message + " (позиция " + std::to_string(position_ + 1) + ")");
    }

    void skipSpace() {
        while (position_ < text_.size() && std::isspace(static_cast<unsigned char>(text_[position_]))) ++position_;
    }

    bool take(char expected) {
        skipSpace();
        if (position_ < text_.size() && text_[position_] == expected) {
            ++position_;
            return true;
        }
        return false;
    }

    std::unique_ptr<TreeNode> parseChild() {
        if (!take('(')) fail("ожидалась запись потомка");
        skipSpace();
        if (position_ < text_.size() && text_[position_] == ')') {
            ++position_;
            return nullptr;
        }
        --position_;
        return parseNode();
    }

    std::unique_ptr<TreeNode> parseNode() {
        if (!take('(')) fail("ожидалась открывающая скобка '('");
        skipSpace();
        const size_t start = position_;
        if (position_ < text_.size() && (text_[position_] == '-' || text_[position_] == '+')) ++position_;
        const size_t digits = position_;
        while (position_ < text_.size() && std::isdigit(static_cast<unsigned char>(text_[position_]))) ++position_;
        if (digits == position_) fail("ожидалось целое число");

        int value;
        try {
            value = std::stoi(text_.substr(start, position_ - start));
        } catch (...) {
            fail("число вне диапазона типа int");
        }

        auto current = std::make_unique<TreeNode>(value);
        if (take('(')) {
            --position_;
            current->left = parseChild();
            if (take('(')) {
                --position_;
                current->right = parseChild();
            }
        }
        if (take('(')) fail("у узла больше двух потомков: это не двоичное дерево");
        if (!take(')')) fail("ожидалась закрывающая скобка ')'");
        return current;
    }
};

void depthFirstRecursive(const TreeNode* node) {
    if (!node) return;
    std::cout << node->key << ' ';
    depthFirstRecursive(node->left.get());
    depthFirstRecursive(node->right.get());
}

struct AvlNode {
    int key;
    int height = 1;
    int count = 1;
    std::unique_ptr<AvlNode> left, right;
    explicit AvlNode(int value) : key(value) {}
};

class AvlTree {
public:
    void insert(int key) { root_ = insert(std::move(root_), key); }
    void erase(int key) { root_ = erase(std::move(root_), key); }

    bool contains(int key) const {
        const AvlNode* node = root_.get();
        while (node) {
            if (key == node->key) return true;
            node = key < node->key ? node->left.get() : node->right.get();
        }
        return false;
    }

    void breadthFirst() const {
        if (!root_) return;
        std::queue<const AvlNode*> nodes;
        nodes.push(root_.get());
        while (!nodes.empty()) {
            const AvlNode* node = nodes.front();
            nodes.pop();
            print(node);
            if (node->left) nodes.push(node->left.get());
            if (node->right) nodes.push(node->right.get());
        }
    }

    void preorder() const {
        if (!root_) return;
        std::vector<const AvlNode*> stack{root_.get()};
        while (!stack.empty()) {
            const AvlNode* node = stack.back();
            stack.pop_back();
            print(node);
            if (node->right) stack.push_back(node->right.get());
            if (node->left) stack.push_back(node->left.get());
        }
    }

    void inorder() const {
        std::vector<const AvlNode*> stack;
        const AvlNode* node = root_.get();
        while (node || !stack.empty()) {
            while (node) {
                stack.push_back(node);
                node = node->left.get();
            }
            node = stack.back();
            stack.pop_back();
            print(node);
            node = node->right.get();
        }
    }

    void postorder() const {
        if (!root_) return;
        std::vector<const AvlNode*> stack{root_.get()}, reversed;
        while (!stack.empty()) {
            const AvlNode* node = stack.back();
            stack.pop_back();
            reversed.push_back(node);
            if (node->left) stack.push_back(node->left.get());
            if (node->right) stack.push_back(node->right.get());
        }
        for (auto it = reversed.rbegin(); it != reversed.rend(); ++it) print(*it);
    }

private:
    std::unique_ptr<AvlNode> root_;

    static int height(const std::unique_ptr<AvlNode>& node) { return node ? node->height : 0; }
    static int balanceFactor(const AvlNode* node) { return height(node->left) - height(node->right); }
    static void updateHeight(AvlNode* node) { node->height = 1 + std::max(height(node->left), height(node->right)); }
    static void print(const AvlNode* node) {
        for (int i = 0; i < node->count; ++i) std::cout << node->key << ' ';
    }

    static std::unique_ptr<AvlNode> rotateRight(std::unique_ptr<AvlNode> y) {
        auto x = std::move(y->left);
        y->left = std::move(x->right);
        updateHeight(y.get());
        x->right = std::move(y);
        updateHeight(x.get());
        return x;
    }

    static std::unique_ptr<AvlNode> rotateLeft(std::unique_ptr<AvlNode> x) {
        auto y = std::move(x->right);
        x->right = std::move(y->left);
        updateHeight(x.get());
        y->left = std::move(x);
        updateHeight(y.get());
        return y;
    }

    static std::unique_ptr<AvlNode> rebalance(std::unique_ptr<AvlNode> node) {
        if (!node) return node;
        updateHeight(node.get());
        if (balanceFactor(node.get()) > 1) {
            if (balanceFactor(node->left.get()) < 0) node->left = rotateLeft(std::move(node->left));
            return rotateRight(std::move(node));
        }
        if (balanceFactor(node.get()) < -1) {
            if (balanceFactor(node->right.get()) > 0) node->right = rotateRight(std::move(node->right));
            return rotateLeft(std::move(node));
        }
        return node;
    }

    static std::unique_ptr<AvlNode> insert(std::unique_ptr<AvlNode> node, int key) {
        if (!node) return std::make_unique<AvlNode>(key);
        if (key < node->key) node->left = insert(std::move(node->left), key);
        else if (key > node->key) node->right = insert(std::move(node->right), key);
        else { ++node->count; return node; }
        return rebalance(std::move(node));
    }

    static AvlNode* minimum(AvlNode* node) {
        while (node->left) node = node->left.get();
        return node;
    }

    static std::unique_ptr<AvlNode> erase(std::unique_ptr<AvlNode> node, int key) {
        if (!node) return node;
        if (key < node->key) node->left = erase(std::move(node->left), key);
        else if (key > node->key) node->right = erase(std::move(node->right), key);
        else {
            if (node->count > 1) { --node->count; return node; }
            if (!node->left) return std::move(node->right);
            if (!node->right) return std::move(node->left);
            AvlNode* successor = minimum(node->right.get());
            node->key = successor->key;
            node->count = successor->count;
            successor->count = 1;
            node->right = erase(std::move(node->right), successor->key);
        }
        return rebalance(std::move(node));
    }
};

void copyIntoAvl(const TreeNode* node, AvlTree& avl) {
    if (!node) return;
    avl.insert(node->key);
    copyIntoAvl(node->left.get(), avl);
    copyIntoAvl(node->right.get(), avl);
}

int main() {
    std::string notation;
    std::cout << "Введите скобочную запись дерева, пример: (8 (9 (5)) (1))\n";
    if (!std::getline(std::cin, notation)) {
        std::cerr << "Ошибка: введите скобочную запись дерева в первой строке.\n";
        return 1;
    }
    try {
        auto tree = TreeParser(notation).parse();
        std::cout << "Обычное дерево (рекурсивный обход в глубину): ";
        depthFirstRecursive(tree.get());
        std::cout << '\n';

        AvlTree avl;
        copyIntoAvl(tree.get(), avl);
        std::cout << "АВЛ, обход в ширину: "; avl.breadthFirst(); std::cout << '\n';
        std::cout << "АВЛ, прямой обход (preorder): "; avl.preorder(); std::cout << '\n';
        std::cout << "АВЛ, симметричный обход (inorder): "; avl.inorder(); std::cout << '\n';
        std::cout << "АВЛ, обратный обход (postorder): "; avl.postorder(); std::cout << '\n';
        std::cout << "Команды: + число (вставка), - число (удаление), ? число (поиск), q (выход).\n";

        char command;
        while (std::cin >> command && command != 'q') {
            if (command != '+' && command != '-' && command != '?') {
                std::cout << "Неизвестная команда\n";
                std::string rest;
                std::getline(std::cin, rest);
                continue;
            }
            int value;
            if (!(std::cin >> value)) { std::cout << "Ожидалось целое число\n"; return 1; }
            if (command == '+') { avl.insert(value); std::cout << "Добавлено\n"; }
            else if (command == '-') { avl.erase(value); std::cout << "Удалено, если ключ существовал\n"; }
            else std::cout << (avl.contains(value) ? "Ключ найден\n" : "Ключ не найден\n");
            std::cout << "АВЛ (inorder): "; avl.inorder(); std::cout << '\n';
        }
    } catch (const std::exception& error) {
        std::cerr << "Ошибка разбора: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
