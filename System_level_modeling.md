# System level modelling

This markdown logs down the experiment where the whole system will be simulated to provide us with important information we need.

For example, the FIFO size we need.

## 16 July

After the meeting on Tuesday 15 July, I started looking at the possibility of SystemC. It seems it is doable, and everything will be carried out and simulated by clock cycles. 

But the whole system will be written in C++, the data visualisation will be difficult together with signal processing.

But I still managed to build up the system C environment on my ubuntu machine where I simulated a simple counter and print out the signals in the waveform.

![Counter behaviour level modelling system C](./img/simple_counter_simulation_using_systemC.png)


## 17 July

I just learnt a long way around where you can simulate system C using xcelium.

You can of course simulate a whole system C project.

![a Vending machine simulation with hierarchy](./img/total_systemC_project_with_vending_machines_and_such_using_xrun.png)


The top test file is located in the [folder](./vending_machine/test_drink.cpp), to simulate it, simply run the command.

```tcl
xmsc_run *.cpp -layout cdebug -top test_drink -64BIT -scfrontend &
```

And then I also tried to simulate a verilog testbench with system c module instantiated.


The testbench can be found here, to simulate it, you need to run the command:

```tcl
xrun -svscdi -nocopyright -libverbose tb_fsm.v ./rtl/FSM_stub.v ./sc/FSM.cpp -64 -sysc
```

**System C file is defined in FSM.cpp, but direct porting from system C to verilog is not allowed, you need to create a verilog shell for it to run**

In this case FSM_stub.v is the verilog shell for FSM.


To instantiate verilog module in system C, you have to follow the definition below:

```verilog
///
// vchild.v (Instantiated Verilog module)
/// 
 `timescale 1ns/1ns
module vchild(in, out);
  input in;
  output out;
  reg outReg;
  initial begin
       outReg = 1'b0;
   #10 outReg = 1'b1;
   #10 outReg = 1'bx;
   #10 outReg = 1'bz;
   #10 outReg = 1'b0;
   #10 outReg = 1'b1;
   #10 outReg = 1'bx;
   #10 outReg = 1'bz;
   #10 $finish;
  end

  assign out = outReg;

  initial $monitor($time,,"in = %b, out = %b", in, out)
endmodule
```

```c++
//////
// sctop.h. (SystemC .h file)
/////

#include "systemc.h"

/* SystemC shell module for HDL module */
class vchild : public xmsc_foreign_module {
public:
  sc_in<sc_logic> in;
  sc_out<sc_logic> out;
  vchild(sc_module_name nm) : xmsc_foreign_module(nm),
   in("in"), out("out") { }
  const char* hdl_name() const { return "vchild"; } // XM mode name
};

/* Uses the shell module in another SystemC module */

class sctop : public sc_module {
public:
  SC_CTOR(sctop): port("port"), vlocal("vlocal") {
   vlocal.in(port);
   vlocal.out(port);
}
protected:
  vchild          vlocal;
  sc_signal<sc_logic> port;
};
```

```c++

//////
// sctop.cpp (SystemC .cpp file)
//////
#include "sctop.h"
XMSC_MODULE_EXPORT(sctop)
```

In the meantime, I made a very simple fifo module in python, which is written as a class.

Now I just have to do it in a very simple fashion.

I will try to hook them up tomorrow.


## 18 July

I have added a method to the Row based encoder 5P, now it can encode the data live, where it will record the repeating pattern and take 1 line of sensor data at a time.

After a simple test with the original encode_in_mem method, it proves to be the same results.

Now it will be better integrated with the data pipeline and we will see how they will be hooked to the FIFO.


## 21 July

I will now hook the data line up with the original Quantised image-->compression unit-->FIFO and track the FIFO


**Data used for this experiment is the most dense image of 500 per cc and 30 um particles!!!**


So apparently, after the simulation, it seems that if we keep the writing speed at 40MHz and reading at 20MHz, all we ever need is probably just 2 words for the FIFO.

![It seems the smallest ever space left for this case is 255](./img/Buffer_space_avaiable_where_original_space_is_256.png)

#### What if we lower the reading frequency?

Considering our case if the LVDS IP is purchased, and it can handle up to 600 Mbps data transmission.

And there is a 16-bit to serial converter before it, which means it can send out 37.5 Million word per second. or 37.5 MHz.

And we are sharing one LVDS with 10 channels, therefore, each channel will get equivalent of 3.75 MHz reading speed at maximum.

Assume we read at 2.5 MHz for each channel, that will be 1/8 of the producing speed.

Simulate that again now...

Okay, with the worst channel of channel 1, we have the simulation of the worst case of 98 words used.

But the plot indicates an ever-increasing trend of the space that is used by the look of the plot.

![the plot shows an ever-increasing trend of the used up FIFO space](./img/channel_1_with_the_worst_case_where_used_up_spcae_keeps_increasing_with_seemingly_no_end.png)


#### Same case but read at 3.333333 MHz

If we read the FIFO at 3.33333 MHz, i.e. 1/6 of the writing rate. this would still be a problem probably......

![Writing to reading ratio at 6 is still not gonna help](./img/reading_at_3_33333_MHz_still_does_not_address_the_problem_with_ever_increasing_need_for_space.png)

#### Then how fast do we need to prevent overflowing?

Because the way this has been set up, I can only make the write to read ratio an integer. 

But so far, the conclusion is the write to read ratio should be **between 4 and 5**

**Case with 4**

![This is the used up space of the worst case when reading at 5 MHz](./img/When_write_read_ratio_is_4_the_worst_case_calls_for_40_words.png)

**Case with 5**

![This is the case when the worst case calls for 60 words at channel 1 when reading at 4 MHz](./img/Write_to_read_speed_ratio_at_5_aka_reading_at_4_MHz_this_would_still_be_a_problem.png)

Conclusion:

To prevent overflow, we might need the reading speed at around 5MHz for each channel.


## 22 July

After the experiment from yesterday, we can see that for an image with only 1000 lines, the buffer size of 256 is still enough.

But for a foreseeable future, it will run out very quickly.

I will generate a much longer image to test it out.

But also, what if we introduce the 1-bit encoding to the process?

I will first update the channel class to get the filter added.

The simple one-bit mode has been added to the channel class

## 23 July

I have just generated a longer image with 100 pixels and 20,000 rows.

Will quantise it and then feed it into the channel class to see how it will react.

Also just realised the runs I made yesterday has the configuration of **500 per cc** and mean size of **20 um**

With the testing of the following configuration:

+ writing speed at 20MHz, sometimes 1 word, sometimes 2 words
+ FIFO depth 256
+ FIFO width 16
+ reading speed at 5MHz always 1 word at a time
+ arm separation 0.2

256 words are enough for this case, max word use is 78.


When reading speed is 4 MHz, 256 FIFO depth is still enough and max word use is 135

![The 4MHz reading speed can work under the stress of 20k rows of image max space taken is 135](./img/Reading_at_4Mz_the_max_word_used_is_135_at_test_of_20k_rows_test.png)

When reading speed is 3.33333 MHz, 256 FIFO depth is **NOT** enough, but 512 would suffice. The max word use is 415.

However, this does not look safe:

![Maximum used up space across all channels: 415](./img/Channel_10_with_an_ever_increasing_word_use_of_FIFO_max_at_415.png)

I think this agrees with the conclusion made from yesterday where the safe reading speed should be no less than 4 MHz.

But for a less dense case like when arm separation is 0.06, even 128 would be sufficient for reading speed at: 3.33 MHz, 2.86 MHz and 2.5 MHz.

I will test out the case when FIFO is full, if the 1-bit mode will help.

## 24 July

Since we have had the test where 256-word deep FIFO is not enough for the reading speed of 3.3333 MHz.

I will try to turn the one bit mode on and see if that will help.

I have recreated the case for reading speed of 3.33 MHz with one-bit-mode turned on. 

It seems this cannot change the fact that it will be full eventually.

![Turned one bit mode on and it seems this does not substantially change the situation but only postponed the inevitable](./img/one_bit_mode_turned_on_with_reading_at_3_33MHz_and_postponed_the_filling_of_FIFO.png)

With one-bit mode turned on, it only extended the filling of the FIFO by few time steps.

10281 - 10268 = 13

only 13 steps were saved, this is not helpful in this case.

We should either rethink about this one-bit mode or just not do it at all in this case.

In the meantime, I will generate a new batch of data with the configuration of **500 per cc** and mean size of **30 um**


## 25 July

To clarify, when I say "rethink about one-bit mode", I mean thinking of something that would actually help save space.

Still waiting for the image generation to complete....

Image generation has completed, the test on these images showed that the encoder will struggle with images where arm separation is 0.2. will plot out the image and compare to see if the image is too dense in the normal context.

## 28 July

Just wrote up the arbiter class and encapsulated the whole 5 channels and arbiter into one data line class.

So far, the arbiter will only pop data of each FIFO by order. If the FIFO is empty, it will skip this FIFO and do nothing.

Therefore, the arbiter will work in the same clock cycle as each diode producer, i.e. 20 MHz.

Test on 20k row image with the particle density of **500 per cc** and mean size of **30 um**

The test is based off the following configuration:

+ writing speed at 20MHz, sometimes 1 word, sometimes 2 words
+ FIFO depth 256
+ reading speed at 20MHz round robin among 5 channels
+ arm separation 0.06

So far, the images under the construction of 5 channels per arbiter round robin has been working alright with image 006 and the max space used across the data line is 80 words.

![The data line class shows that the round robin has been okay for image 006 with longer 20k row images](./img/Image_006_simulation_for_the_DataLine_where_sinmple_round_robin_dataloop_has_been_included.png)


But if I set up the Data line to run arm separation of 0.2, it will fail pretty quickly with all the FIFOs filled up.


![Arm Separation of 0.2 proves to be very difficult and the solution of compression and round robin data pop is difficult to clear out the data](./img/image02_proves_to_be_more_challenging_for_this_architecture_where_round_robin_is_not_gonna_help.png)


## 29 July

Following the meeting today, I will list up all the possible arbiter mechanism I can think of right now:

#### Weighted Round Robin

Assign weights based on FIFO fullness, more full FIFOs get more priority

Problem here: You need to track FIFO fullness...


#### Urgency-Based Arbitration

Prioritise FIFOs that are close to full

Compare FIFO fill levels each cycle and assign priority to the FIFO with the highest occupancy.

Some kinda of FIFO usage level may be needed, or "almost_full" flag, and how much should one session read at a time?


#### Priority + Ageing

Assign static priorities but gradually increase "urgency" for low-priority FIFOs that haven't been served in a while.


#### Token Bucket

Give each FIFO a token bucket, a FIFO can transmit only if it has tokens.



## 30 July

So I checked quickly, it is possible to have a quick conversion from Gray code back to Binary. 

Conversion should look like:

```text
B[n-1] = G[n-1]                   // MSB is the same
B[i]   = B[i+1] XOR G[i]          // for i = n-2 down to 0
```

This translates to the following verilog, if there are fixed 4 bits of gray code:

```verilog
B[3] = G[3];
B[2] = G[3] ^ G[2];
B[1] = G[3] ^ G[2] ^ G[1];
B[0] = G[3] ^ G[2] ^ G[1] ^ G[0];
```


## 12 Aug

I have the Urgency based algorithm written in verilog, the overall algorithm is defined as follow:


Input:

Space used across 5 different channels --> data_used [4:0]

FIFO empty flags --> fifo_empty [4:0]

Output:

Grant_one_hot --> grant_one_hot [4:0]


Internal register:

urgency [4:0]
age [4:0]
grant [2:0]


```text

for id in range(5):
  if(fifo_empty[id])
    age[id] = 0
    urgency[id] = 0
  else if(grant == id)
    age[id] = 0
  else if(age[id] < 255)
    age[id] += 1

  urgency[id] = (data_used[id] << 2) + age [id]

grant = maxid(urgency)
grant_one_hot = OneHot(grant) 
```

## 14 Aug

I will write up a new arbiter class based on the old round robin arbiter class.

It will be called ArbiterUrgency and it should have the same methods as the base class **arbiter**.

I finished drafting for the class ArbiterUrgency, will now plug it in to test the stressful cases.

And it appears that this arbiter does a better job at keeping all the channels fair and tried to pop the data whenever it could.

But due to the high throughput of the channel, it simply cannot cope the demand.

![The urgency based arbiter is treating each channel fair and square, but cannot cope the large data throughput](./img/urgency_based_arbiter_does_a_better_job_at_keeping_each_channel_fair.png)


# System level modeling Continued

This project mainly carries previous test and research work with continued sight and view point.

But still, this will be mainly focused on the compression algorithms implementation, either on purely software or hardware implementation.

## 05 Dec 2025

Following the meeting on 04 Dec, it has been pointed out that we should have a more thorough simulation on the data flow for the pixel and compressor.

Another thing we talked about is the 3-bit 1-bit mode, which should be implemented in the class to verify the effectiveness for it. Cus my old 1-bit mode simply just squeezes the 3 rows' pixel values into the 16-bit data without any optimisation. This might change if it is actually needuseful to have the 1-bit mode integrated into the compressor.

**1-bit mode? What is the direction?**

What was mentioned about the 1-bit mode is that we shall keep the compression algorithm we have. Instead, we squeeze 3 rows of pixels into 1 word instead of each row for 1 word.

But how do we categorise 7 values into 2?

What Piotr talked about is: simply take the MSB as the 1-bit input. For example, *1*00 --> *1*, *1*01 --> *1*, *0*11 --> *0*, *0*10 --> *0*.

So that the binary code kept the range half half. i.e. 0-3 --> 0; 4-7 --> 1;, which is what we want.

*But will this work for Gray code?*

TL;DR. Yes, technically it will work. But this depends on our threshold definition.

There are 2 aspects to consider:

1. Will Gray code representations of number 0-3 and 4-7 be correctly identified into 0 and 1?
2. Will the final binary form truly represent the light intensity at either < 0.5 and >= 0.5?


Question 1: Yes it will.

|      |      |      |      |      |      |      |      |      |
|------|------|------|------|------|------|------|------|------|
|  Bin |  000 | 001  |  010 | 011  | 100  | 101  | 110  | 111  |   
| Gray |  000 | 001  |  011 | 010  | 110  | 111  | 101  | 100  | 
|  Binarised value    |  0 | 0  | 0  | 0  |  1  | 1 |  1 | 1| 

This is because Gray code calculation is: A ^ (A >> 1)

So Gray code for 0xx: 0xx ^ 00x --> 0xx

And Gray code for 1xx: 1xx ^ 01x --> 1xx


Question 2: It may not truly represent the true intensity if the binary representation has not been designed 50/50.

If the original binary representation has not been designed in 50/50, then naturally the binary representation will not be categorised around the threshold 0.5.

That is:
```text
##############
# Ideal case #
##############
                0.5
                 |
                 v

  001  010  011    100  101  110  111

#####################
# Not so ideal case #
#####################

               0.5
                |
                v
  
 000  001  010     011  100  101  110
```


## 08 Dec 2025

I have reviewed previous simulations I have implemented for the entire data line where 5 groups of pixels, 5 compressors, 5 FIFOs and 1 arbiter were encapsulated.

The previous implementation works by:

On each loop (slice of image), the compressor will compress the image for its own channel and push the result into the respective FIFO.

--> The selected channel will be popped by 1 word.

This means my previous simulation has the writing and popping at the same frequency.

If writing at 20 MHz, reading will also be 20 MHz.



## 10 Dec 2025

I will now consider if what was mentioned on [08 Dec 2025](#08-dec-2025) is the real case for our application.

There are 2 things that works in a different way as we would expect.

+ There will be 8 channels inside one LVDS/arbiter
+ The FIFO/arbiter read speed could actually be slightly faster than 20 MHz but not faster than 37.5 MHz.

I will now try to construct the async dataline class that can write and pop data in different rate.

With a common tick method, I can achieve this. 

For example:

```text

If writing at 20 MHz, and reading at 37.5 MHz.

Then for the period of 0.4 us, there will be 8 writings and 15 readings.

0.4 e-6 / (1/20 MHz) = 8
0.4 e-6 / (1/37.5 MHz) = 15

I could increment the common tick with a time step of 0.003333333 us, and then every time it counts 15, we do one writing operation, and every time it counts to 8, we do one reading operation.
```

This will be constructed in the new class.

## 11 Dec 2025

Following yesterday's idea, I will proceed the implementation for the new python class.

Unlike previous dataline implementation, where it will simply do the data production and consumption at the same time. This new class has 2 different methods to produce and consume the data in FIFOs.

Hence they can be operated at different rates in a separated method.

In the previous dataline class, the simulation runs live on a "time_step", which syncs with the line of pixel values production.

But in our new class, this will not work, so I will keep those 2 methods work on different "time_step". Each method's "time_step" will reflect respective "production" or "consumption" cycle.

Other 2 methods where FIFO's space was checked will be kept since they work independently from the time step and can be checked statically.

Now I have finished the construction of the class, I will do a simple test on the class with a single slice of image of 40 pixels.

I have tested the async data line with the following test configurations:
+ Total running pixels length: 20 K 
+ Particle density: 500 cc 
+ Average particle size: 30 um 
+ Arm separation: 0.06
+ Arbiter: Round robin skip empty
+ Producing speed: 20 MHz 
+ Consuming speed: 37.5 MHz
+ FIFO depth: 256


Under such testing conditions, the FIFOs were all well-managed and did not overflow.



![8 channels FIFO used up space track under the simple async data line test](./img/used_up_space_track_for_a_single_image_slice_during_Async_dataline_simulation_11_Dec_2025.png)

The maximum used up FIFO belongs to channel 6 with 36 words used. This result agrees with the simulation result from [28 July](#28-july) where a simple image sample from the same big image was presented to the channel of 5.

But will it survive the arm separation of 0.2?

TL;DR; Nope, it will not.

Similarly to the test carried out on the same day, the results shows that the FIFOs will fill up eventually and the reading speed cannot catch up with the production.

Conclusion: our system should be okay if we keep the configurations as what we have tested. But the system cannot handle images when the arm separation is raised up to 0.2.


## 12 Dec 2025

I have verified the channel will survive the image of 0.06 for 500 cc and 30 um, but it will not survive the 0.2 image no matter what.

So here I shall plot out the example image of those two and see if this is acceptable for Jonny.

![20k run images with different arm separations side by side comparison](./img/20k_run_image_006_02_side_by_side_comparison.png)

It can be seen that when arm separation is 0.2, the image has become much more messy than the image 006, which could be a problem to deal with under our case.


Steve and I have also discussed the interface for the outputs of his ADC.

The waveform form Steve's side would look something like this:

**NOTE: The nStrobe signal will actually only last for 3-4 ns**

**Disclaimer: nStrobe signal in the waveform is for demonstration purpose only**

![3 important signals from Steve's side waveform might look like this](./img/pixel_ADC_from_steve_side_with_important_3_signals.png)


## 17 Dec 2025

To answer the question for thresholding and impact from the small particles on the final image.

I have applied the possible thresholds using the quantiser_3B class and plotted out corresponding images after quantisation.

I will include them into a spread sheet.

**WARNING: The quantised values in the image represent the light intensity**

First we have the thresholds of following:

**001 thresholds: [0.13, 0.19, 0.25, 0.31, 0.38, 0.44, 0.5]**

The same images look like this:

![same segment of the 20k images with the thresholds combo of 001](./img/ThresholdingCombo001SameSegmentImagsQuan3B.png)


And after having different thresholds to quantise the images, these images have been put under test for the compression and async FIFO pop

For thresholds combo 001, it can handle both arm separation of 006 and 02.

Test results of image 006, max use for fifo is 14 words:

![](./img/FIFO_of_8_channels_under_tests_of_quan_image_threshold_001_image_006.png)

Test results of image 02, max use for fifo is 95 words:

![](./img/FIFO_of_8_channels_under_tests_of_quan_image_threshold_001_image_02.png)

For thresholds combo 010, it can only handle arm separation of 006, but cannot handle 02.


Test results of image 006, max use for fifo is 32 words:

![](./img/FIFO_of_8_channels_under_tests_of_quan_image_threshold_010_image_006.png)

Test result for image 02 will not be shown, cus it is not successful.

