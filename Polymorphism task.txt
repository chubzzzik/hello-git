#include <iostream>
#include<iomanip>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <memory>
using namespace std;

const double PI = 3.14159;

class Shape{
    public:
    virtual double area() const  = 0;
};

class Circle : public Shape{
    double radius;
    public:
    Circle(double r): radius(r){}

    double area() const override {
        return(PI*radius*radius);
    }
};


class Rectangle : public Shape{
    double width;
    double length;
    public:
    Rectangle(double w, double l): width(w),length(l){}

    double area() const override{
        return(length*width);
    }

};

int main()
{
    cout << fixed << setprecision(2);
    int N;
    cin >> N;
    vector<unique_ptr<Shape>> mass(N);

    for(int i = 0; i<N; i++){
        string input_commmand;
        double arg, arg_2;
        cin >> input_commmand;
        if(input_commmand == "circle"){
            cin >> arg;
            mass[i] = (make_unique<Circle>(arg));
        }
        else if(input_commmand == "rectangle"){
            cin >> arg;
            cin >> arg_2;
            mass[i] = (make_unique<Rectangle>(arg,arg_2));
        }
    }
    for(const auto& Shape : mass){
        cout << Shape->area() << endl;
    }
    return 0;
}
