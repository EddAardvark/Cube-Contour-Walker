#include "VLInt.h"


void VLInt::Test()
{
    // AddInt

    __int64 to_add[11] = { -5, -1, 0, 1, 5, 9, 10, 12, 24, 33, 111 };
    __int64 start[3] = { -99, 0, 99 };

    // Basic Addition

    for (auto i = 0; i < 3; ++i)
    {
        __int64 x = start[i];
        VLInt vlx(start[i]);

        if (vlx.ToInt() != x)
        {
            std::ostringstream sstrm;

            sstrm << "Initialise vlx " << start[i] << " returned " << vlx;

            throw std::exception(sstrm.str().c_str());
        }

        for (auto j = 0; j < 11; ++j)
        {
            auto was = x;
            x += to_add[j];
            vlx = vlx + to_add[j];

            if (vlx.ToInt() != x)
            {
                std::ostringstream sstrm;

                sstrm << "Add: " << was << " + " << to_add[j] << ", got " << vlx << ", expected " << x;

                throw std::exception(sstrm.str().c_str());
            }
        }
    }

    // Basic Subtraction

    for (auto i = 0; i < 3; ++i)
    {
        __int64 x = start[i];
        VLInt vlx(start[i]);

        for (auto j = 0; j < 11; ++j)
        {
            auto was = x;
            x -= to_add[j];
            vlx = vlx - to_add[j];

            if (vlx.ToInt() != x)
            {
                std::ostringstream sstrm;

                sstrm << "Add: " << was << " - " << to_add[j] << ", got " << vlx << ", expected " << x;

                throw std::exception(sstrm.str().c_str());
            }
        }
    }

    // Factorials (multiply by int)

    std::string f40 = "815915283247897734345611269596115894272000000000";

    static VLInt fn[41];

    fn[0] = VLInt(1);

    for (int i = 1; i <= 40; ++i)
    {
        fn[i] = fn[i - 1] * i;
    }

    if (fn[40].ToString() != f40)
    {
        throw std::exception("Factorial 40");
    }

    // Factorials (multiply by VLInt)

    VLInt fact(1);

    for (int i = 1; i <= 40; ++i)
    {
        VLInt vli(i);
        fact = fact * vli;

        if (fact != fn[i])
        {
            std::stringstream sstrm;
            sstrm << "Factorial [" << i << "] " << fact << " != " << fn[i];
            throw std::exception(sstrm.str().c_str());
        }
    }

    // Division by VLInt

    for (int i = 0; i < 40; ++i)
    {
        int n = 40 - i;
        VLInt vli(n);

        fact = fact / vli;

        if (fact != fn[n - 1])
        {
            std::stringstream sstrm;
            sstrm << "Factorial Division [" << i << "] " << fact << " != " << fn[n - 1];
            throw std::exception(sstrm.str().c_str());
        }
    }

    // Cubes, Mod 9, Mod 3 + increment

    VLInt vli(1000000);
    static int mod9[3] = { 0, 1, 8 };

    for (int i = 0; i < 40; ++i)
    {
        auto c = vli.Cube();
        auto m9 = c.Mod9();
        auto m3 = vli.Mod3();

        if (m9 != mod9[m3])
        {
            std::stringstream sstrm;
            sstrm << "Modulo test: " << vli << " cubed = " << c << ", mod 9 = " << m9 << ", mod 3 = " << m3 << std::endl;
            throw std::exception(sstrm.str().c_str());
        }

        ++vli;
    }

    // Decrement

    for (int i = 0; i < 40; ++i)
    {
        --vli;
    }

    if (vli.ToInt() != 1000000)
    {
        std::stringstream sstrm;
        sstrm << "Decrement test: " << vli << " !- " << 1000000 << std::endl;
        throw std::exception(sstrm.str().c_str());
    }

    // Log 10

    vli = VLInt(2);
    auto log2 = log10(2.0);
    auto check = (int)((log2 - (int)round(log2)) * 100000000);

    for (int i = 0; i < 40; ++i)
    {
        double l2 = vli.Log10();
        int ipart = (int)round(l2);
        double fpart = l2 - ipart;
        int ifpart = (int)(fpart * 100000000);

        if (ifpart != check || ipart != i)
        {
            std::stringstream sstrm;
            sstrm << "Power test: " << "Log (" << vli << ") = " << l2 << ": expected " << ipart << " == " << i << " and " << ifpart << " == " << check << std::endl;
            throw std::exception(sstrm.str().c_str());
        }
        vli = vli * 10;
    }

    // Ratio & Pow, should give 1.099511627776

    VLInt two(2);
    VLInt ten(10);
    auto rcheck = 99511627776L;

    ten = ten.Pow(12);
    two = two.Pow(40);

    auto rat = Ratio(two, ten);
    __int64 rchk2 = (__int64)((rat - 1) * 1000000000000);

    if (rcheck != rchk2)
    {
        std::stringstream sstrm;
        sstrm << "Ratio test: " << two << " / " << ten << " = " << rat << " (" << rcheck << " != " << rchk2 << ") " << std::endl;
        throw std::exception(sstrm.str().c_str());
    }

    // Finished

    std::cout << "VLInt: All tests passed." << std::endl;
}

