#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cfloat>
#include <string>

#include <ctime>  // for timing purposes
#include <vector>
#include <complex>

#include "cmdline.hpp"
#include "vector.hpp"
#include "complex.hpp"
#include "dft.hpp"
#include "errors.hpp"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::fstream;
using std::ios;
using std::istringstream;
using std::string;

static void opt_input(string const &);
static void opt_output(string const &);
static void opt_method(string const &);
static void opt_help(string const &);

// syntax: {needs argument,
//          short name,
//          long name,
//          default value,
//          parser,
//          flag}
//
static option_t options[] = {
    {1, "i", "input", "-", opt_input, OPT_DEFAULT},
    {1, "o", "output", "-", opt_output, OPT_DEFAULT},
    {1, "m", "method", "DFT", opt_method, OPT_DEFAULT},
    {0, "h", "help", NULL, opt_help, OPT_DEFAULT},
    {0, },
};

static istream *iss = 0;
static ostream *oss = 0;
typedef Vector<Complex> (* transform_method)(const Vector<Complex> &v);
static transform_method transform;
static fstream ifs;
static fstream ofs;


static void opt_input(string const &arg) {
    if (arg == "-") {
        iss = &cin;
    } else {
        ifs.open(arg.c_str(), ios::in);
        iss = &ifs;
    }

    if (!iss->good()) {
        cerr << ERROR_MSG_CANT_OPEN_FILE
             << arg
             << "."
             << endl;
        exit(1);
    }
}

static void opt_output(string const &arg) {
    if (arg == "-") {
        oss = &cout;
    } else {
        ofs.open(arg.c_str(), ios::out);
        oss = &ofs;
    }

    if (!oss->good()) {
        cerr << ERROR_MSG_CANT_OPEN_FILE
             << arg
             << "."
             << endl;
        exit(1);
    }
}

static void opt_method(string const &method) {
    if (method == "DFT") {
        transform = DFT::transform;
    } else if (method == "IDFT") {
        transform = DFT::inverse;
    } else {
        cerr << ERROR_MSG_UNKNOWN_METHOD << endl;
        opt_help("");
    }
}

static void opt_help(string const &arg) {
    cout << HELP_MSG
         << endl;
    exit(0);
}

int main(int argc, char * const argv[]) {
    cmdline cmdl(options);
    cmdl.parse(argc, argv);

    Vector<Complex> v;
    // std::vector< std::complex<double> > y;
    clock_t begin = clock();

    if ((*iss >> v).bad()) {
        cerr << ERROR_MSG_CORRUPTED_DATA << endl;
    }
    // for (std::complex<double> t; *iss >> t;)
    //    y.push_back(t);

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "Tardó " << elapsed_secs << "segundos" << endl;
    std::cout << "Cantidad de elementos: " << v.getSize() << std::endl;

    begin = clock();
    DFT::fft(v);

    end = clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "FFT tardó " << elapsed_secs << "segundos" << endl;

    //v[S,1,5,2];
    //*oss << DFT::ditfft2(v) << endl;
    Vector<Complex> c;
    c.append(Complex(1,0));
    c.append(Complex(1,0));
    c.append(Complex(1,0));
    c.append(Complex(1,0));
    // *oss << "v = " << v << endl;
    // Vector<Complex> whatever = DFT::fft(v);
    // *oss << "fft(v) = " << whatever << endl;
    // *oss << "ifft = " << DFT::ifft(whatever) << endl;

    return 0;
}
