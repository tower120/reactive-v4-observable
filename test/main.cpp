#include <iostream>

#include <Observable.h>
#include <ObservableValue.h>
#include <observe.h>

using namespace reactive::v4;

void test_Observable(){
    class Font : public Observable<reentrant>{
        bool m_bold = false;
        bool m_italic = false;
        bool m_strike = false;

        void update_native(){
            bytes = 1 * m_bold + 10 * m_italic + 100 * m_strike;
        }
    public:
        void bold(bool b){
            // reentrant version
            schedule([&, b](){
                m_bold = b;
                update_native();
                onChange();
            });
        }
        bool bold() const{
            return m_bold;
        }

        void italic(bool b){
            m_italic = b;
            update_native();
            onChange();
        }



        unsigned int color;

        unsigned int bytes;
    };


    Font font1;

    bool b{false};
    font1.onChange.subscribe([&](){
        b = !b;
        if (b){
            return unsubscribe;
        }
        std::cout << "A: " << font1.bytes << std::endl;
        return proceed;
    });

    font1.onChange.subscribe([&](){
        if (font1.bold()){
            font1.schedule([&](){
                font1.bold(false);
            });

        }
        std::cout << "B: " << font1.bytes << std::endl;
    });

    font1.onChange.subscribe([&](){
        std::cout << "C: " << font1.bytes << std::endl;
    });

    font1.bold(true);
    font1.italic(true);



    // copy test
    std::cout << "font1: " << font1.bytes << std::endl;


    font2.onChange.subscribe([&](){
        std::cout << "font2: " << font2.bytes << std::endl;
    });

    font2.italic(false);
    font2 = font1;
    //font2.onChange();
};



void test_ObservableValue(){
    ObservableValue<int> i{1};

    auto tag = i.onChange += [&](){
        std::cout << i << std::endl;
    };

    i = 2;
    i.onChange -= tag;
    i = 3;

    ObservableValue<int, reentrant> i2;
    i2.onChange += [&](){
        std::cout << "i2 = " << i2 << std::endl;
        if (i2 < 10) {
            i2 = i2 + 1;
        }
    };
    i2.onChange += [&](){
        std::cout << "II) i2 = " << i2 << std::endl;
    };

    i2 = i;

    i2 = 11;
    i2 = 11;

}


void test_observe(){
    ObservableValue<int> x,y, len;

    observe([&](){
        len = x+y;
        if (len == 40) return unsubscribe;
        return proceed;
    }, x.onChange, y.onChange);

    len.onChange += [&](){
        std::cout << "len = " << len << std::endl;
    };

    x = 1;
    y = 2;
    x = 38;
    y = 10;
}


int main() {
    //test_Observable();
    //test_ObservableValue();
    test_observe();

#ifdef _MSC_VER
	char ch;
	std::cin >> ch;
#endif

    return 0;
}
