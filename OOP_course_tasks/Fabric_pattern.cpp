#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <memory>

using namespace std;

class Warrior{
    public:
    virtual void info() const = 0;
    virtual ~Warrior() = default;
};

class Infantryman : public Warrior{
    public:
    void info() const override{
        cout << "Пехотинец (Человек)\n";
    }
};

class Archer : public Warrior{
    public:
    void info() const override{
        cout << "Лучник (Человек)\n";
    }
};

class Horseman : public Warrior{
    public:
    void info() const override{
        cout << "Всадник (Человек)\n";
    }
};

class ElfInfantryman : public Warrior{
    public:
    void info() const override{
        cout << "Пехотинец (Эльф)\n";
    }
};

class ElfArcher : public Warrior{
    public:
    void info() const override{
        cout << "Лучник (Эльф)\n";
    }
};

class ElfHorseman : public Warrior{
    public:
    void info() const override{
        cout << "Всадник (Эльф)\n";
    }
};


class ArmyCreator{
    public:
    virtual unique_ptr<Warrior> createInfantryman() const = 0;
    virtual unique_ptr<Warrior> createArcher() const = 0;
    virtual unique_ptr<Warrior> createHorseman() const = 0;
};

class HumanArmyCreator : public ArmyCreator{
    public:
    unique_ptr<Warrior> createInfantryman() const override{
        return make_unique<Infantryman>();
    }
    unique_ptr<Warrior> createArcher() const override{
        return make_unique<Archer>();
    }
    unique_ptr<Warrior> createHorseman() const override{
        return make_unique<Horseman>();
    }
};

class ElfArmyCreator : public ArmyCreator{
    public:
    unique_ptr<Warrior> createInfantryman() const override{
        return make_unique<ElfInfantryman>();
    }
    unique_ptr<Warrior> createArcher() const override{
        return make_unique<ElfArcher>();
    }
    unique_ptr<Warrior> createHorseman() const override{
        return make_unique<ElfHorseman>();
    }
};


int main()
{
    string race;
    cin >> race;
    if(race == "human"){
        unique_ptr<ArmyCreator> creator = make_unique<HumanArmyCreator>();
        auto soldier1 = creator->createInfantryman();
        auto soldier2 = creator->createArcher();
        auto soldier3 = creator->createHorseman();
        soldier1->info();
        soldier2->info();
        soldier3->info();
    }
    else if(race == "elf"){
        unique_ptr<ArmyCreator> creator = make_unique<ElfArmyCreator>();
        auto soldier1 = creator->createInfantryman();
        auto soldier2 = creator->createArcher();
        auto soldier3 = creator->createHorseman();
        soldier1->info();
        soldier2->info();
        soldier3->info();
    }
    return 0;
}
