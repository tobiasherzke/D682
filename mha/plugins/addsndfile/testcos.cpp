#include "addsndfile.cpp"

int main(int, char**) {
    mhaconfig_t cf;
    cf.channels = 2;
    cf.domain = MHA_WAVEFORM;
    cf.fftlen = 512;
    cf.fragsize = 200;
    cf.srate = 44100;
    cf.wndlen = 400;
    mha_real_t adapt_len = 0;
    mha_real_t l_new_ = 2e-5*pow(10.0,0.05*37);
    mha_real_t l_old_ = 2e-5*pow(10.0,0.05*94);

    addsndfile::level_adapt_t adapt(cf, adapt_len, l_new_, l_old_);
    adapt.update_frame();
    float expected = 0.0014158915687682763;
    printf("expected:%.9g, actual:%.9g, difference:%.9g\n", expected, adapt.buf[0], expected-adapt.buf[0]);
    return fabs(expected-adapt.buf[0]) > 1e-6;
}
