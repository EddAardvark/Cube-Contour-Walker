#include "BigCube.h"

//------------------------------------------------------------------------------------------------------
void BigCube::Test()
{
    BigCube bc(1000000000);

    bc.Verify("Test 1");

    VLInt vli(1000000000);
    auto vli3 = vli.Cube();

    if (vli3 != bc.value)
    {
        std::stringstream sstrm;

        sstrm << bc << " != " << vli3;
        throw std::exception(sstrm.str().c_str());
    }

    VLInt eleven(11);
    BigCube bc2(eleven);

    if (bc2.value != 1331)
    {
        std::stringstream sstrm;

        sstrm << "11^3 != 1331 , got " << bc2;
        throw std::exception(sstrm.str().c_str());
    }

    VLInt m20(-20);
    BigCube bc3(m20);

    if (bc3.value != -8000)
    {
        std::stringstream sstrm;

        sstrm << "(-20)^3 != -8000 , got " << bc3;
        throw std::exception(sstrm.str().c_str());
    }
    // Finished

    std::cout << "BigCube: All tests passed." << std::endl;
}
//------------------------------------------------------------------------------------------------------
void BigCube::Verify(const char* where) const
{
    // y = n^3;
    // y' = 3xn^2 + 3xn + 1
    // y'' = 6xn + 6
    // y''' = 6;

    auto n = root;
    auto n2 = n * n;
    auto val = n2 * n;
    auto t_dy = (n2 + n) * 3 + 1;
    auto t_ddy = (n + 1) * 6;

    if (val != value)
    {
        std::stringstream sstrm;

        sstrm << where << ": Value " << value << " != " << val << ", Detail = " << FullText();
        throw std::exception(sstrm.str().c_str());
    }

    if (t_dy != dy)
    {
        std::stringstream sstrm;

        sstrm << where << ": Dy " << dy << " != " << t_dy << ", Detail = " << FullText();
        throw std::exception(sstrm.str().c_str());
    }

    if (t_ddy != ddy)
    {
        std::stringstream sstrm;

        sstrm << where << ": Ddy " << ddy << " != " << t_ddy << ", Detail = " << FullText();
        throw std::exception(sstrm.str().c_str());
    }
}
//--------------------------------------------------------------------------------------------
std::string BigCube::FullText() const
{
    std::stringstream sstrm;

    sstrm << "BC: n = " << root << ", n^3 = " << value << ", dy = " << dy << ", ddy = " << ddy << ", dddy = 6";
    return sstrm.str();
}
