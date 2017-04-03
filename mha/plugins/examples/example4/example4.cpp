// This file is part of the HörTech Open Master Hearing Aid (openMHA)
// Copyright © 2004 2005 2006 2007 2009 2010 2014 2015 HörTech gGmbH
//
// openMHA is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, version 3 of the License.
//
// openMHA is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License, version 3 for more details.
//
// You should have received a copy of the GNU Affero General Public License, 
// version 3 along with openMHA.  If not, see <http://www.gnu.org/licenses/>.

// mhainfo: plugin
/*
 * An simple example MHA plugin written in C++
 *
 * This plugin scales one channel of the input signal, working in the
 * time domain. The scale factor and the scaled channel number is made
 * accessible to the configuration structure.
 */

#include <cstdio>
#include "mha_plugin.hh"

/** A Plugin class using the spectral signal.
 * This is the fourth example plugin for the step-by-step tutorial.
 */
class example4_t : public MHAPlugin::plugin_t<int> {
    /** Index of audio channel to scale. */
    MHAParser::int_t scale_ch;
    /** The scaling factor applied to the selected channel. */
    MHAParser::float_t factor;
    /** Keep Track of the prepare/release calls */
    MHAParser::int_mon_t prepared;

    /** The Event connector */
    MHAEvents::patchbay_t<example4_t> patchbay;

    /* Callbacks triggered by Events */
    void on_scale_ch_writeaccess();
    void on_scale_ch_valuechanged();
    void on_scale_ch_readaccess();
    void on_prereadaccess();
public:
    /** This constructor initializes the configuration language
     * variables and inserts them into the MHA configuration tree.
     * It connects the MHA Events triggered by these configuration variables
     * to the respective callbacks. */
    example4_t(algo_comm_t & ac,
               const std::string & chain_name,
               const std::string & algo_name);

    /** Plugin preparation. This plugin checks that the input signal
     * has the waveform domain and contains enough channels.
     * @param signal_info 
     *   Structure containing a description of the form of the signal
     *   (domain, number of channels, frames per block, sampling rate.
     */
    void prepare(mhaconfig_t & signal_info);

    /** Bookkeeping only. */
    void release(void);

    /** Signal processing performed by the plugin.  
     * This plugin multiplies the spectral signal in the selected
     * audio channel by the configured factor.
     * @param signal
     *   Pointer to the input signal structure.
     * @return
     *   Returns a pointer to the input signal structure,
     *   with a the signal modified by this plugin.
     *   (In-place processing)
     */
    mha_spec_t * process(mha_spec_t * signal);
};

example4_t::example4_t(algo_comm_t & ac,
                       const std::string & chain_name,
                       const std::string & algo_name)
    : MHAPlugin::plugin_t<int>("This plugin multiplies the sound signal"
                          " in one audio channel by a factor.  It works"
                   " in the spectral domain.",ac),
      scale_ch("Index of audio channel to scale. Indices start from 0."
               " Only channels with even indices may be scaled.",
               "0",
               "[0,["),
      factor("The scaling factor that is applied to the selected channel.",
             "0.1", 
             "[0,["),
      prepared("State of this plugin: 0 = unprepared, 1 = prepared")
{
    insert_item("channel", &scale_ch);
    insert_item("factor", &factor);
    prepared.data = 0;
    insert_item("prepared", &prepared);
    
    patchbay.connect(&scale_ch.writeaccess, this,
                     &example4_t::on_scale_ch_writeaccess);
    patchbay.connect(&scale_ch.valuechanged, this,
                     &example4_t::on_scale_ch_valuechanged);
    patchbay.connect(&scale_ch.readaccess, this,
                     &example4_t::on_scale_ch_readaccess);
    patchbay.connect(&scale_ch.prereadaccess, this,
                     &example4_t::on_prereadaccess);
    patchbay.connect(&factor.prereadaccess, this,
                     &example4_t::on_prereadaccess);
    patchbay.connect(&prepared.prereadaccess, this,
                     &example4_t::on_prereadaccess);
}

void example4_t::prepare(mhaconfig_t & signal_info)
{
    if (signal_info.domain != MHA_SPECTRUM)
        throw MHA_Error(__FILE__, __LINE__,
                        "This plugin can only process spectrum signals.");
    // The user may have configured scale_ch before prepare is called.
    // Check that the configured channel is present in the input signal.
    if (signal_info.channels <= unsigned(scale_ch.data))
        throw MHA_Error(__FILE__,__LINE__,
                        "This plugin requires at least %d input channels.",
                        scale_ch.data + 1);
    // bookkeeping
    prepared.data = 1;
}

void example4_t::release(void)
{
    prepared.data = 0;
}

mha_spec_t * example4_t::process(mha_spec_t * signal)
{
    unsigned int bin;
    // spectral signal is stored non-interleaved.
    mha_complex_t * channeldata =
        signal->buf + signal->num_frames * scale_ch.data;
    for(bin = 0; bin < signal->num_frames; bin++)
        channeldata[bin] *= factor.data;
    return signal;
}

void example4_t::on_scale_ch_writeaccess()
{
    printf("Write access: Attempt to set scale_ch=%d.\n", scale_ch.data);
    // Can be used to track any writeaccess to the configuration, even
    // if it does not change the value.  E.g. setting the name of the
    // sound file in a string configuration variable can cause a sound
    // file player plugin to start playing the sound file from the
    // beginning.
}
void example4_t::on_scale_ch_valuechanged()
{
    if (scale_ch.data & 1)
        throw MHA_Error(__FILE__,__LINE__,
                        "Attempt to set scale_ch to non-even value %d",
                        scale_ch.data);
    // Can be used to recompute a runtime configuration only if some
    // configuration variable actually changed.
}
void example4_t::on_scale_ch_readaccess()
{
    printf("scale_ch has been read.\n");
    // A configuration variable used as an accumulator can be reset
    // after it has been read.
}
void example4_t::on_prereadaccess()
{
    printf("A configuration language variable is about to be read.\n");
    // Can be used to compute the value on demand.
}

MHAPLUGIN_CALLBACKS(example4,example4_t,spec,spec)
MHAPLUGIN_DOCUMENTATION(example4,"example","")

// Local Variables:
// compile-command: "make"
// c-basic-offset: 4
// indent-tabs-mode: nil
// coding: utf-8-unix
// End:
