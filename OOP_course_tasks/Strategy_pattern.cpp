#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <memory>

class DeliveryStrategy {
    public:
    virtual double calculateCost(double weight) const = 0;
    virtual ~DeliveryStrategy() = default;
};

class CourierDelivery:public DeliveryStrategy{
    public:
    double calculateCost(double weight) const override{
        return 5.0 + weight * 0.5;
    }
};

class PostalDelivery:public DeliveryStrategy{
    public:
    double calculateCost(double weight) const override{
        return 2.0 + weight * 0.2;
    }
};

class PickupStrategy:public DeliveryStrategy{
    public:
    double calculateCost(double weight) const override{
        return 0.0;
    }
};

class Order{
    private:
    std::unique_ptr<DeliveryStrategy> strategy;
    double weight;
    
    public:
    Order(double w):weight(w){}

    void setDelivery(std::unique_ptr<DeliveryStrategy> s){
        strategy = std::move(s);
    }

    double getTotalCost()const{
        double cost = strategy ? strategy->calculateCost(weight) : 0.0;
        return cost;
    }
};

int main(){
    std::cout << std::fixed << std::setprecision(2);
    std::string deliveryType;
    double weight;
    getline(std::cin,deliveryType);
    std::cin >> weight;
    Order order(weight);

    if(deliveryType == "courier"){
        order.setDelivery(std::make_unique<CourierDelivery>());
    }
    else if(deliveryType == "post"){
        order.setDelivery(std::make_unique<PostalDelivery>());
    }
    else if(deliveryType == "pickup"){
        order.setDelivery(std::make_unique<PickupStrategy>());
    }

    std::cout << order.getTotalCost();
    return 0;
}