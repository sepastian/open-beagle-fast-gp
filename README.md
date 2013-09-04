open-beagle-fast-gp
===================

Open BEAGLE fast GP (FGP) is an extension for Open BEAGLE to speedup fitness evaluation in GP (genetic programming) training.

Fitness evaluation using FGP is 8-10 times faster than standard Open BEAGLE.
This is achieved by compiling binary versions of all individuals for fitness evaluation.

Use this software at your own risk.
It lacks documentation and rigourous testing; from a GP perspective, the training parameters are far form good.

Feedback is very welcome!


Installation
------------

FGP requires [OpenBEAGLE-4.0.0-alpha2](https://code.google.com/p/beagle/), which in turn requires [pacc-1.4.1](http://sourceforge.net/projects/pacc/).

  wget https://code.google.com/p/beagle/downloads/detail?name=OpenBEAGLE-4.0.0-alpha2-Source.tar.gz
  tar xf OpenBEAGLE-4.0.0-alpha2-Source.tar.gz
  cd OpenBEAGLE-4.0.0-alpha2-Source.tar.gz
  cmake .
  make
  sudo make install  # install into /usr/local

  wget https://code.google.com/p/beagle/downloads/detail?name=pacc-1.4.1-Source.tar.gz
  tar xf pacc-1.4.1-Source.tar.gz
  cd pacc-1.4.1-Source.tar.gz
  cmake .
  make
  sudo make install  # install into /usr/local

  git clone https://github.com/sepastian/open-beagle-fast-gp
  cd open-beagle-fast-gp
  cmake .
  make
  ./gp  # success!

Usage
-----

TODO


Fitness Evaluation in Open BEAGLE
---------------------------------

Open BEAGLE creates C++ functors representing the GP operators.
Each individual is represented as a tree, where each node contains the name of an operator as a string and leafs contain values.
Evaluating the fitness of an individual involves mapping operator names to functors, and invoking the functors.
Consequently, evaluating thousands of individuals per generation on large data sets, with each indidivual potentially consisting of hundreds of nodes, can become very slow.

(To be fair, while rather slow, Open BEAGLE offers much flexibility and is a great tool for exploring different methods of evolutinary algorithms.)


Other Methods of Fitness Evaluation
-----------------------------------

To tackle the speed-issue, other (proprietary) GP engines use a different approach.
Each individual is represented (or mapped to) assembler instructions.
During fitness evaluation, the assembler instructions are copied to memory and a C-style pointer is used to invoke the assembler program.
This approach is much faster then the string-lookup methods used in Open BEAGLE.
However, using assembler instructions has several drawbacks of its own: a very close bond to the CPU architectures used for training is created; supporting different CPU architectures is tiresome and often not possible at all; debugging errors in assembler code loaded into memory dynamically is not great fun (believe me).
Finally, from a business perspective, when assembler code is used in a classifier in production, the same CPU architecture that has been used for training must be used in production.
However, this is often not possible, because the target environment is using different types of CPUs.


Yet Another Method of Fitness Evaluation
----------------------------------------

Faced with this problem and taking into account the time fitness evaluation is taking in Open BEAGLE vs. the problems introduced by using assembler instructions, the idea of FGP took shape.
FGP is an extension for Open BEAGLE; but instead of using string lookups for fitness evaluation, a shared library containing all individuals is compiled and used.
At each generation, the individuals constructed during Open BEAGLE's GP training process are parsed and translated into C code, resulting in one function per individual.
Next, all functions are compiled into a single shared library; a special evaluation operator for Open BEAGLE is used to classify the training set using the individuals found in the shared library.
The fitness results for all individuals are fed back into Open BEAGLE and training continues with the next generation.
This approach is 8-10 times faster than fitness evaluation of GP individuals in standard Open BEAGLE.
At the same time, no assembler code is used and the classifiers trained are platform independent.

In addition to fitness evaluation using shared libraries, MCC (Matthew's Correlation Coefficient) has been implemented.
MCC is not part of standard Open BEAGLE and well suited for handling binary classification data sets with very unbalanced class sizes, e.g. 99 % of all individuals belonging to class A, 1 % of all individuals belonging to class B.
