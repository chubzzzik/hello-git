#include<iostream>
#include <iomanip>

class ITemperatureSensor{
    public:
    virtual double getTemperatureCelsius()const = 0;
};


class FahrenheitSensor {
    public:
    double getFahrenheit() const{
        return 77.0;
    }
};

class TemperatureAdapter:public ITemperatureSensor{
    private:
    const FahrenheitSensor& source_;
    public:
    TemperatureAdapter(const FahrenheitSensor& source):source_(source){}

    double getTemperatureCelsius() const override{
        double temp = source_.getFahrenheit();
        return (temp - 32.0) * 5.0 / 9.0;
    }
};

void controlSystem(double output){ 
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "Температура: " << output << " degC\n";
}

int main(){
    FahrenheitSensor sensor;
    TemperatureAdapter adapter(sensor);
    controlSystem(adapter.getTemperatureCelsius());
    return 0;
}