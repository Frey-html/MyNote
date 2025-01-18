**Q 1**
**a.**
By Amdahl's Law, Speedup:
$$
S = \frac{1}{(1-0.8) + \frac{0.8}{2}}=\frac{5}{3}
$$
Then frequency can be decrease:
$$
F_{\text{new}} = \frac{F_{\text{old}}}{S}
$$
Therefore:
$$
\frac{F_{\text{new}}}{{F_{\text{old}}}} = \frac{1}{S} =0.6
$$

**b.**
$$
P_{dual}​=0.5*C*(0.6V_{original​})^2⋅(0.6f_{original}​)=0.216P_{original}
$$
Thus, the dual-core system requires 21.6% of the dynamic power compared to the single-core system.

**c.**
We can know that 
$$
V_{new} = 0.3V_{old}
$$
$$
S = \frac{1}{(1-P)+\frac{P}{2}} = \frac{1}{\frac{V_{new}}{V_{old}}} = 3.3 
$$
So that 
$$
P = 1.4
$$
Since P cannot be greater than 1, this indicates that the voltage floor constraint is not feasible for this level of parallelization


**d.**
$$
P_{Dual} = 0.5*C*({0.3V^2_{origin}})*(0.3f_{origin})=0.3^3P_{origin}
$$
Thus the dual-core system with the voltage floor will require 2.7% of the dynamic power compared to the single-core system


**Q 2**
- For a large number of identical and independent components, the MTTF of the system can be approximated by:
$$
MTTF_{system}≈\frac{MTTF_{\text{computer}}}{Fraction\:of\:computers\:failing}
$$
$$
MTTF_{\text{system}} ​≈ {\text{90days}}
​​$$

**Q 3**
The number of transistors on a microchip doubles approximately every two years, while the cost of computing power decreases.

**Q 4**
Amdahl's Law is a formula used to predict the potential speedup of a task when a portion of it is parallelized. It states that the overall improvement gained by parallelizing part of a program is limited by the fraction of the task that remains sequential (cannot be parallelized).
The law is expressed as:
$$
S = \frac{1}{(1 - P) + \frac{P}{N}}
$$

where:
- S is the speedup,
- P is the proportion of the task that can be parallelized,
- N is the number of processors.

**Q 5**
An Instruction Set Architecture (ISA) defines the set of instructions that a processor can execute and how it interacts with the system. It typically includes the following components:
Instructions, Registers, Addressing Modes, Data Types, Instruction Formats, Memory Architecture and Control Flow.

**Q 6**
We start by calculating the failure rate (λ) for each component from its MTTF:
$$
λ_{\text{system}}​=\frac{10}{1000,000}+\frac{1}{500,000}​+\frac{1}{200,000}​+\frac{1}{200,000}+\frac{1}{1,000,000}
$$
$$
\lambda_{\text{system}} = \frac{10 + 2 + 5 + 5 + 1}{1{,}000{,}000} = \frac{23}{1{,}000{,}000}
$$
$$
MTTF_{\text{system}}​=\frac{1}{\lambda_{\text{system}}} ≈ {\text{43478hours}}​
$$

**Q 7**
$$
S = \frac{1}{\frac{0.5}{10}+0.5} ≈ 1.818
$$

**Q 8**
The overall baseline CPI is:
$$
\text{CPI}_{\text{total}} = f_{\text{FP}} \cdot \text{CPI}_{\text{FP}} + f_{\text{Non-FP}} \cdot \text{CPI}_{\text{Non-FP}}
$$
$$
\text{CPI}_{\text{total}} = 0.25 \cdot 4.0 + 0.75 \cdot 1.33 = 1.0 + 0.9975 = 1.9975
$$
**a. alternative 1**
$$
\text{CPI}_{\text{FP, new}} = f_{\text{FSQRT}} \cdot \text{CPI}_{\text{FSQRT, new}} + f_{\text{Other FP}} \cdot \text{CPI}_{\text{Other FP}}
$$
$$
\text{CPI}_{\text{FP, new}} = 0.02 \cdot 1.5 + 0.23 \cdot 4.0 = 0.03 + 0.92 = 0.95
$$
$$
\text{CPI}_{\text{total, new}} = f_{\text{FP}} \cdot \text{CPI}_{\text{FP, new}} + f_{\text{Non-FP}} \cdot \text{CPI}_{\text{Non-FP}}
$$
$$
\text{CPI}_{\text{total, new}} = 0.25 \cdot 0.95 + 0.75 \cdot 1.33 = 0.2375 + 0.9975 = 1.235
$$
**b. alternative 2**
$$
\text{CPI}_{\text{total, new}} = f_{\text{FP}} \cdot \text{CPI}_{\text{FP, new}} + f_{\text{Non-FP}} \cdot \text{CPI}_{\text{Non-FP}}
$$
$$
\text{CPI}_{\text{total, new}} = 0.25 \cdot 2.0 + 0.75 \cdot 1.33 = 0.5 + 0.9975 = 1.4975
$$
Thus, alternative 1 (decreasing the CPI of FSQRT to 1.5) gives a better performance improvement, as it results in a lower overall CPI