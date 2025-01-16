#### Q1
**（ a ）** 
FLOPs per processor per cycle= 8×32
For 10 SIMD processors :
Total FLOPs per cycle = 10×8×32=256 FLOPs per cycle

Throughput (FLOP/s)=$$256×0.8×0.7×0.85×1.5×10^{9}=182.79×10^9FLOP/s=182.79GFLOP/s$$

**（ b ）** 
1. 2
2. 1.5
3. 0.95/0.85 = 1.11


#### Q2
$512 \div (8 * 8) = 8$
A 512-bit-wide vector can operate on 8 integers at once
The number of instruction needs = 524 / 8 = 65.5, round to 66
So clocks = 66 * 4 = 264


#### Q3
A B A


#### Q5
**（ a ）** For each complex result, we need 4 loads, 4 multiplications and 2 additions/subtractions, 1 store
total for each complex number:
(15 + 3) cycles+32 cycles+10 cycles+15 cycles=75 cycles per complex result

**（ b ）**
each complex result need 4 loads
total for each complex number:
(15 + 1) cycles+32 cycles+10 cycles+15 cycles=73 cycles per complex result

