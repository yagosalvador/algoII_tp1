#ifndef FT_HPP__
#define FT_HPP__

#include <cfloat>  // For DBL_EPSILON
#include <cmath>
#include <iostream>
#include "vector.hpp"
#include "complex.hpp"

using std::string;

// #define DFT_TOLERANCE 10E-4
// #include <vector>
// #include <complex>
// using std::vector;
// using std::complex;
// using std::pow;


class FourierTransform
{
public:
	~FourierTransform(){};
	virtual Vector<Complex> transform(const Vector<Complex>& x)=0;
};

class DFT: public FourierTransform{
 public:
    virtual Vector<Complex> transform(const Vector<Complex>& x) {
        Vector<Complex> y = Vector<Complex>(x);

        size_t N = x.getSize();
        double argument = -(2*M_PI)/N;
        Complex wn(cos(argument), sin(argument));

        // for (Vector<Complex>::iterator yi = y.begin(); yi != y.end(); ++yi) {
        //    size_t i = yi - y.begin();
        //    *yi = Complex();
        //    for (Vector<Complex>::iterator xj = x.begin(); xj != x.end(); ++xj) {
        //        size_t j = xj - x.begin();
        //        *yi += (*xj)*(wn^(i*j));
        //    }

        // Vector<Complex>::iterator y_start = y.begin();
        // Vector<Complex>::iterator x_start = x.begin();
        // Vector<Complex>::iterator y_end = y.begin();
        // Vector<Complex>::iterator x_end = x.begin();
        // for (Vector<Complex>::iterator yi = y_start; yi != y_end; ++yi) {
        //    size_t i = yi - y_start;
        //    *yi = Complex();
        //    for (Vector<Complex>::iterator xj = x_start; xj != x_end; ++xj) {
        //        size_t j = xj - x_start;
        //        *yi += (*xj)*(wn^(i*j));
        //    }

        size_t y_size = y.getSize();
        size_t x_size = x.getSize();
        for (size_t yi = 0; yi < y_size; ++yi) {
            y[yi] = Complex();
            for (size_t xj = 0; xj < x_size; ++xj) {
                y[yi] += x[xj]*(wn^(yi*xj));
            }

            //  Sets to 0 components that are almost 0
            // if (std::abs((*yi).getReal()) < DFT_TOLERANCE)
            //     (*yi).setReal(0);
            // if (std::abs((*yi).getImag()) < DFT_TOLERANCE)
            //     (*yi).setImag(0);
        }

        return y;
    }

    // static Vector<Complex> ditfft2(const Vector<Complex> &x) {
    //     Vector<Complex> y = Vector<Complex>(x);

    //     size_t N = y.getSize();
    //     if (N == 1) {
    //         return y;
    //     }
    //     else {
    //         Vector<Complex> even = y.slice(0, -1, 2);  // take the even indexes
    //         Vector<Complex> odd = y.slice(1, -1, 2);  // take the odd indexes

    //         even = ditfft2(even);
    //         odd = ditfft2(odd);
    //         y = even + odd;

    //         for (size_t i = 0; i < N/2; ++i) {
    //             double argument = -(2*M_PI*i)/N;
    //             Complex twiddle(cos(argument), sin(argument));
    //             Complex even = y[i];
    //             Complex odd = twiddle*y[i+N/2];

    //             y[i] = even + odd;
    //             y[i+N/2] = even - odd;
    //         }
    //     }
    //     return y;
    // }
};
class IDFT: public FourierTransform{
  public:
    virtual Vector<Complex> transform(const Vector<Complex>& x){
        Vector<Complex> y = Vector<Complex>(x);

        size_t N = x.getSize();
        double argument = (2*M_PI)/N;
        Complex wn(cos(argument), sin(argument));
        for (Vector<Complex>::iterator yi = y.begin(); yi != y.end(); ++yi) {
            size_t i = yi - y.begin();
            *yi = Complex();
            for (Vector<Complex>::iterator xj = x.begin(); xj != x.end(); ++xj) {
                size_t j = xj - x.begin();
                *yi += (*xj)*(wn^(i*j));
            }
            *yi /= N;

            //  Sets to 0 components that are almost 0
            // if (std::abs((*yi).getReal()) < DFT_TOLERANCE)
            //     (*yi).setReal(0);
            // if (std::abs((*yi).getImag()) < DFT_TOLERANCE)
            //     (*yi).setImag(0);
        }

        return y;
    }

};

class FFT: public FourierTransform
{
public:
	    static virtual Vector<Complex> transform(const Vector<Complex> &x, double w = 2*M_PI) {
        Vector<Complex> y = Vector<Complex>(x);

        size_t N = y.getSize();
        if (N == 1) {
            return y;
        }
        Vector<Complex> even = y.slice(0, -1, 2);  // take the even indexes
        Vector<Complex> odd = y.slice(1, -1, 2);  // take the odd indexes
        even = transform(even, w);
        odd = transform(odd, w);
        //y = even + odd;

        for (size_t i = 0; i < N/2; ++i) {
            double argument = -(w*i)/N;
            Complex twiddle(cos(argument), sin(argument));
            odd[i] *= twiddle;

            y[i] = even[i] + odd[i];
            y[i+N/2] = even[i] - odd[i];
        }
        return y;
}

    // static vector< complex<double> > transform(const vector< complex<double> >& x) {
    //     vector< complex<double> > y = vector< complex<double> >(x);

    //     size_t N = x.size();
    //     double argument = -(2*M_PI)/N;
    //     complex<double> wn(cos(argument), sin(argument));

    //     // for (Vector<Complex>::iterator yi = y.begin(); yi != y.end(); ++yi) {
    //     //    size_t i = yi - y.begin();
    //     //    *yi = Complex();
    //     //    for (Vector<Complex>::iterator xj = x.begin(); xj != x.end(); ++xj) {
    //     //        size_t j = xj - x.begin();
    //     //        *yi += (*xj)*(wn^(i*j));
    //     //    }

    //     // Vector<Complex>::iterator y_start = y.begin();
    //     // Vector<Complex>::iterator x_start = x.begin();
    //     // Vector<Complex>::iterator y_end = y.begin();
    //     // Vector<Complex>::iterator x_end = x.begin();
    //     // for (Vector<Complex>::iterator yi = y_start; yi != y_end; ++yi) {
    //     //    size_t i = yi - y_start;
    //     //    *yi = Complex();
    //     //    for (Vector<Complex>::iterator xj = x_start; xj != x_end; ++xj) {
    //     //        size_t j = xj - x_start;
    //     //        *yi += (*xj)*(wn^(i*j));
    //     //    }

    //     size_t y_size = y.size();
    //     size_t x_size = x.size();
    //     for (size_t yi = 0; yi < y_size; ++yi) {
    //         y[yi] = complex<double>(0, 0);
    //         for (size_t xj = 0; xj < x_size; ++xj) {
    //             y[yi] += x[xj]*pow(wn, (yi*xj));
    //         }

    //         //  Sets to 0 components that are almost 0
    //         // if (std::abs((*yi).getReal()) < DFT_TOLERANCE)
    //         //     (*yi).setReal(0);
    //         // if (std::abs((*yi).getImag()) < DFT_TOLERANCE)
    //         //     (*yi).setImag(0);
    //     }

    //     return y;
    // }	
};

class IFFT: public FourierTransform{
  public:
   	virtual Vector<Complex> transform(const Vector<Complex> & x){
 		return FFT::transform(x, -2*M_PI)/x.getSize();   	}
};

#endif  // FT_HPP_
