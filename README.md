# GenericCryptanalyzer
A generic library used for differential cryptanalysis of block ciphers written in C++.

## About
GenericCryptanalyzer is my Bachelor's Thesis project that implements a automated discovery search algorithm of differential characteristics of a block cipher. It uses a branch-and-bound algorithm inspired by Matsui’s Search Algorithm For The Best Differential/Linear Trails, presented in [1].

### Motivation
Differential cryptanalysis was introduced in [5] and Linear cryptanalysis was first introduced in [7]. Over the years theese techniques have proven to be one of the most efficient techniques in searching vulnerabilities in block ciphers.

A good tutorial on both differential and linear cryptanalysis can be found in [6].

As computational power and interest in safe cryptographic protocols has increased over time, automated search for differential and linear characteristics has become a must for both cryptologists and cryptanalysts. This tool aims to help in automated discovery of such vulnerabilities in order to save research time.

## Installation
### Prerequisites
In order to install GenericCryptanalyzer you will need to have installed the following C++ libraries:
* boost

## Examples
Some examples of how to interract with the library can be found in `examples/`.

## Documentation
The documentation for this project can be found [here](https://gabitulba.github.io/GenericCryptanalyzer/).

## Further Improvements
* Finish adding docs for `src/builders/`
* Add more unit tests
* Implement optimisations presented in [2], [3], [4] to improve performance
* Add more types of boxes to support more ciphers
* Add support for linear cryptanalysis

## References
1. [Mitsuru Matsui - On Correlation Between the Order of S-boxes and the Strength of DES - Mitsuru Matsui](https://link.springer.com/content/pdf/10.1007/BFb0053451.pdf)
2. [Improving the Search Algorithm for the Best Linear Expression - Kazuo Ohta, Shiho Moriai & Kazumaro Aoki ](https://link.springer.com/chapter/10.1007/3-540-44750-4_13)
3. [Best Differential Characteristic Search of FEAL - Kazumaro Aoki, Kunio Kobayashi & Shiho Moriai](https://link.springer.com/chapter/10.1007/BFb0052333)
4. [Speeding Up the Search Algorithm for the Best Differential and Best Linear Trails - Zhenzhen Bao, Wentao Zhang & Dongdai Lin](https://www.researchgate.net/publication/301982433_Speeding_Up_the_Search_Algorithm_for_the_Best_Differential_and_Best_Linear_Trails)
5. [Differential Cryptanalysis of DES-like Cryptosystems - Eli Biham & Adi Shamir](https://link.springer.com/article/10.1007/BF00630563)
6. [A Tutorial on Linear and Differential Cryptanalysis - Howard M. Heys](https://ioactive.com/wp-content/uploads/2015/07/ldc_tutorial.pdf)
7. [A New Method for Known Plaintext Attack of FEAL Cipher - Mitsuru Matsui & Atsuhiro Yamagishi](https://link.springer.com/content/pdf/10.1007/3-540-47555-9_7.pdf)
