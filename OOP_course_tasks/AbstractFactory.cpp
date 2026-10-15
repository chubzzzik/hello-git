#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <memory>

using namespace std;

class Processor{
    public:
    virtual void info() = 0;
    virtual ~Processor() = default;
};

class DDR{
    public:
    virtual void info() = 0;
    virtual ~DDR() = default;
};

class HDD{
    public:
    virtual void info() = 0;
    virtual ~HDD() = default;
};

class Gaming_Processor : public Processor{
    public:
    void info() override{
        cout << "Процессор для игрового компьютера" << endl;
    }
};

class Gaming_DDR : public DDR{
    public:
    void info() override{
        cout << "Оперативная память для игрового компьютера" << endl;
    }
};

class Gaming_HDD : public HDD{
    public:
    void info() override{
        cout << "Жесткий диск для игрового компьютера" << endl;
    }
};

class Office_Processor : public Processor{
    public:
    void info() override{
        cout << "Процессор для офисного компьютера" << endl;
    }
};

class Office_DDR : public DDR{
    public:
    void info() override{
        cout << "Оперативная память для офисного компьютера" << endl;
    }
};

class Office_HDD : public HDD{
    public:
    void info() override{
        cout << "Жесткий диск для офисного компьютера" << endl;
    }
};

class ComputerFactory{
    public:
    virtual unique_ptr<Processor> create_processor() = 0;
    virtual unique_ptr<DDR> create_DDR() = 0;
    virtual unique_ptr<HDD> create_HDD() = 0;
    virtual ~ComputerFactory() = default;
};

class GamingComputerFactory : public ComputerFactory{
    public:
    unique_ptr<Processor> create_processor() override{
        return make_unique<Gaming_Processor>();
    };
    unique_ptr<DDR> create_DDR() override{
        return make_unique<Gaming_DDR>();
    }
    unique_ptr<HDD> create_HDD() override{
        return make_unique<Gaming_HDD>();
    };
};

class OfficeComputerFactory : public ComputerFactory{
    public:
    unique_ptr<Processor> create_processor() override{
        return make_unique<Office_Processor>();
    };
    unique_ptr<DDR> create_DDR() override{
        return make_unique<Office_DDR>();
    }
    unique_ptr<HDD> create_HDD() override{
        return make_unique<Office_HDD>();
    };
};

void createComputer(ComputerFactory& factory){
    auto processor = factory.create_processor();
    auto DDR = factory.create_DDR();
    auto HDD = factory.create_HDD();
    processor->info();
    DDR->info();
    HDD->info();

}

int main (){
    GamingComputerFactory gamingfactory;
    OfficeComputerFactory officefactory;
    string type;
    getline(cin,type);
    if(type == "gaming"){
        createComputer(gamingfactory);
    }else{
    createComputer(officefactory);
    }
    
    return 0;
}