/* -*- c++ -*- */
/*
 * Copyright 2023 bowei.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "rx_frame_equalization_csi_impl.h"

namespace gr {
  namespace mmwave_bw {

    using input_type = gr_complex;
    using output_type = gr_complex;
    rx_frame_equalization_csi::sptr
    rx_frame_equalization_csi::make(int num_ant, int num_stm, float samp_rate, int fft_size, int num_ofdm_per_frame, int dec_alg, int step_size, int vht_en, int num_sc_pha_noise)
    {
      return gnuradio::make_block_sptr<rx_frame_equalization_csi_impl>(
        num_ant, num_stm, samp_rate, fft_size, num_ofdm_per_frame, dec_alg, step_size, vht_en, num_sc_pha_noise);
    }


    rx_frame_equalization_csi_impl::rx_frame_equalization_csi_impl(int num_ant_, int num_stm_, float samp_rate_, int fft_size_, int num_ofdm_per_frame_, int dec_alg_, int step_size_, int vht_en_, int num_sc_pha_noise_)
      : gr::block("rx_frame_equalization_csi",
              gr::io_signature::make(1 /* min inputs */, 8 /* max inputs */, sizeof(input_type)),
              gr::io_signature::make(1 /* min outputs */, 8 /*max outputs */, sizeof(output_type)))
    {
    	p.initialize(num_ant_,samp_rate_,fft_size_,num_ofdm_per_frame_,vht_en_,num_sc_pha_noise_);
    	dec_alg=dec_alg_;
    	data=new_vec_1(cpx,p.num_ant*p.fft_size*p.num_total_ofdm_per_frame,gr_complex(0,0));
    	set_output_multiple(p.fft_size*p.num_total_ofdm_per_frame);
    }

    /*
     * Our virtual destructor.
     */
    rx_frame_equalization_csi_impl::~rx_frame_equalization_csi_impl()
    {
    }

    void
    rx_frame_equalization_csi_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      for(int i=0;i<p.num_ant;i++)
      	ninput_items_required[i] = p.fft_size*p.num_total_ofdm_per_frame;
    }

    int
    rx_frame_equalization_csi_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      for(int i=0;i<p.num_ant;i++){
        in[i] = (input_type *) input_items[i];
      }
      for(int i=0;i<p.num_ant+1;i++){
      	out[i] = (output_type *) output_items[i];
      }
      
      if (dec_alg==1)
      	lft_based_mrc_equalization();
      if (dec_alg==2)
      	mimo_zero_forcing();
      
      //phase_detect();
      
      memcpy(&out[0][0],&data[0],sizeof(gr_complex)*p.fft_size*p.num_total_ofdm_per_frame*p.num_ant);
      produce(0,p.fft_size*p.num_total_ofdm_per_frame*p.num_ant);
      
      /*
      for(int i=0;i<p.num_ant;i++){
        memcpy(&out[i+1][0],&h[i][0],sizeof(gr_complex)*p.fft_size);
      	produce(i+1,p.fft_size);
      }
      */
      consume_each (p.fft_size*p.num_total_ofdm_per_frame);
      // Tell runtime system how many output items we produced.
      return WORK_CALLED_PRODUCE;
    }
    
    
    
    
    
    
    void rx_frame_equalization_csi_impl::lft_based_mrc_equalization(){
    	h=new_vec_2(cpx,p.num_ant,p.fft_size,gr_complex(0,0));
    	int k=0;
    	for(int i=0;i<p.num_ant;i++)
    		for(int j=0;j<p.valid_sc.size();j++){
    			k=p.valid_sc[j];
    			h[i][k]=float(0.25)*(in[i][k]+in[i][p.fft_size+k]+in[i][p.fft_size*2+k]+in[i][p.fft_size*3+k])/p.time_frame_f[k];
    			//cout<<in[i][k]<<"   --->"<<in[i][p.fft_size*1+k]<<"   --->"<<in[i][p.fft_size*2+k]<<"   --->"<<in[i][p.fft_size*3+k]<<"   ->"<<p.time_frame[k]<<"\n";
    		}
    	gr_complex signal_sum;
    	float h_sum;
    	int m=0;
    	
    	for(int k=0;k<p.valid_sc.size();k++){
    		for(int j=0;j<p.num_total_ofdm_per_frame;j++){
    			h_sum=0;signal_sum=gr_complex(0,0);
    			for(int i=0;i<p.num_ant;i++){
    				m=p.valid_sc[k];
    				h_sum+=abs(h[i][m])*abs(h[i][m]);
    				signal_sum+=conj(h[i][m])*in[i][j*p.fft_size+m];
    			}
    			data[j*p.fft_size+m]=signal_sum/h_sum;
    		
    		}
    	}
    	
    
    }
    
     void rx_frame_equalization_csi_impl::mimo_zero_forcing(){
     
     	int k;
     	h_mimo=new_vec_3(cpx,p.num_ant,p.num_ant,p.fft_size,gr_complex(0,0));
     	h_mimo_inver=new_vec_3(cpx,p.num_ant,p.num_ant,p.fft_size,gr_complex(0,0));
     	//calculate the channel information matrix h
     	for(int n=0;n<p.num_ant;n++){
     		for(int i=0;i<p.num_ant;i++){
     			for(int j=0;j<p.valid_sc.size();j++){
     				k=p.valid_sc[j];
     				h_mimo[n][i][k]=(in[n][(3+i)*p.fft_size+k])/p.time_frame_f[k];
     				//cout<<h_mimo[n][i][k]<<"="<<in[n][(3+i)*p.fft_size+k]<<"/"<<p.time_frame_f[k]<<"\n";
     				//cout<<n<<" "<<i<<" "<<k<<"="<<(3+i)*p.fft_size+k<<"/"<<"\n";
     			}
     		}
     	}
     	
     	
     	arma::cx_mat ARMA_H(p.num_ant, p.num_ant);
	for (int j = 0; j < p.valid_sc.size(); j++) {
		k = p.valid_sc[j];
		for(int n = 0; n < p.num_ant; n++) {
			for(int m = 0; m < p.num_ant; m++) 
				ARMA_H(n,m) = h_mimo[n][m][k];
		}
		
		//arma::cx_mat ARMA_H_INV = arma::pinv(ARMA_H);
		/*
		for(int n = 0; n < p.num_ant; n++) {
			for(int m = 0; m < p.num_ant; m++) 
				h_mimo_inver[n][m][k] = ARMA_H_INV(m,n);
		}
		*/
	}
	
	/*
	for (int j = 0; j < p.valid_sc.size(); j++) {
		k = p.valid_sc[j];
			for (int l = 0; l < p.num_total_ofdm_per_frame; l++) {
				for(int m = 0; m < p.num_ant; m++) {
					//cout<<m*p.fft_size*p.num_total_ofdm_per_frame+l*p.fft_size+k<<"\n";
					data[m*p.fft_size*p.num_total_ofdm_per_frame+l*p.fft_size+k] = gr_complex(0, 0);
					//for(int n = 0; n < p.num_ant; n++)				
						 //data[m*p.fft_size*p.num_total_ofdm_per_frame+l*p.fft_size+k] += h_mimo[n][m][k]*in[n][l*p.fft_size+k];
				}
			}
	}
	*/	
	/*	
	for(int i=0;i<p.valid_sc.size();i++){
		k=p.valid_sc[i];
		for(int j=0;j<p.num_total_ofdm_per_frame;j++){
			for(int m=0;m<p.num_ant;m++){
				data[m*p.fft_size*p.num_total_ofdm_per_frame+j*p.fft_size+k]=gr_complex(0,0);
				for(int i=0;i<p.num_ant;i++)
					data[m*p.fft_size*p.num_total_ofdm_per_frame+j*p.fft_size+k]+=h_mimo[m][i][k]*in[i][j*p.fft_size+k];
			}
		}
	}
	*/	
     }
    
    
     void rx_frame_equalization_csi_impl::phase_detect(){
     	float k=0,b=0,ysum,phase_valid;
     	int xsum,l,l_np;
     	vec_1(float) phase=new_vec_1(float,p.pilot_sc.size(),0);
     	vec_1(float) k_np=new_vec_1(float,p.num_ant,0);
     	vec_1(float) b_np=k_np;
     	
     	for(int j=p.num_preamble_ofdm_per_frame;j<p.num_total_ofdm_per_frame;j++){
     		ysum=0,xsum=0;
     		k=0;b=0;
     		for(int i=0;i<p.pilot_sc.size();i++){
     			l=p.pilot_sc[i];
     			phase[i]=arg(data[j*p.fft_size+l]/p.pilot);
			//cout<<phase[i]<<"   "<<data[j*p.fft_size+l]<<l<<"\n";
     			ysum+=phase[i]-phase[0];
     			xsum+=p.pilot_sc_np[i]-p.pilot_sc_np[0];
     		}

     		k+=ysum/xsum;
     		b+=phase[0]-k*p.pilot_sc_np[0];
     		
     		for(int i=0;i<p.valid_sc.size();i++){
     			l=p.valid_sc[i];
     			l_np=p.valid_sc_np[i];
     			phase_valid=-(k*l_np+b);
     			//cout<<phase_valid<<"=-("<<k<<"*"<<l_np<<"+"<<b<<")\n";
     			data[j*p.fft_size+l]*=gr_complex(cosf(phase_valid),sinf(phase_valid));
     		
     		}
     		
     		
     	}

     }

  } /* namespace mmwave_bw */
} /* namespace gr */
