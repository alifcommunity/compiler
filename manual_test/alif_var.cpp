#include <iostream>
#include <string>
#include <cassert>
#include <sstream>

// متغير

class alif_var {
    private:
        double number;
        std::string str;
        void Set(int data) {
            this->number = data;
            std::ostringstream ss;
            ss << data;
            this->str = ss.str();
        }
        void Set(const char* data) {
            this->number = atof(data);
            this->str = data;
        }
        void Set(double data) { 
            this->number = data;
            std::ostringstream ss;
            ss << data;
            this->str = ss.str();
        }
        void Set(std::string data) {
            this->number = atof(data.c_str());
            this->str = data;
        }
    public:
        alif_var() {
            this->number = 0;
            this->str = "";
        }
        alif_var(int data) {
            Set(data);
        }
        alif_var(double data) {
            Set(data);
        }
        alif_var(const char* data) {
            Set(data);
        }
        alif_var(std::string data) {
            Set(data);
        }
    
        // number - Set
        alif_var& operator= (double data) { Set(data); return *this; }
        alif_var& operator>> (double data) { Set(data); return *this; }
        // number - Get
        operator double() { return this->number; }
        double operator++ () { return ++this->number; }
        double operator-- () { return --this->number; }

        // Char - Set
        alif_var& operator= (const char* data) { Set(data); return *this; }
        alif_var& operator>> (const char* data) { Set(data); return *this; }
        // Char - Get
        // ...

        // String - Set
        alif_var& operator= (std::string data) { Set(data); return *this; }
        alif_var& operator>> (std::string data) { Set(data); return *this; }
        // String - Get
        operator std::string() { return this->str; }
};

// -- TEST -------------------------------------------------

double test_double(double v){
    return v * 2;
}

std::string test_string(std::string v){
    std::string buf = "[" + v + "]";
    return buf;
}

alif_var test_var(alif_var v){
    alif_var z;
    z = 1 + v;
    return z;
}

int main() {

    // number
    {
        alif_var a = 1;
        alif_var b = 2;
        alif_var c = a + b;
        std::cout << "c = a + b: " << c << std::endl;
        assert(c == 3);

        double t = test_double(c);
        std::cout << "c * 2: " << t << std::endl;
        assert(t == 6);

        t = test_var(t);
        std::cout << "c + 1: " << t << std::endl;
        assert(t == 7);
    }

    putchar('\n');

    // String
    {
        alif_var a = "1";
        alif_var b = "2";
        alif_var c = a + b;
        std::cout << "c = 'a' + 'b': " << c << std::endl;
        assert(c == 3);

        std::string t1 = test_string(c);
        std::cout << "Text: " << t1 << std::endl;
        assert(t1 == "[3]");

        // alif_var j = std::string("(") + c + std::string(")");
        // std::cout << "Text: " << j << std::endl;
        // assert(j == "(3)");
    }

    // Datatype
    {
        alif_var a = 123;

        int i = a;
        std::cout << "int: " << i << std::endl;
        assert(i == 123);

        float f = a;
        std::cout << "float: " << f << std::endl;
        assert(f == 123.0);

        double d = a;
        std::cout << "double: " << d << std::endl;
        assert(d == 123);
        
        std::string s = a;
        std::cout << "string: " << s << std::endl;
        assert(s == "123");
    }

    return 0;
}
