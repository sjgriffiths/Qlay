# Qlay (C++) tutorial

* [Introduction](#introduction)
* [The quantum bit](#the-quantum-bit)
* [Coding with one qubit](#coding-with-one-qubit)
  * [Setting up an experiment](#setting-up-an-experiment)
  * [Quantum logic gates](#quantum-logic-gates)
  * [Intricacies of Pauli X and Hadamard](#intricacies-of-pauli-x-and-hadamard)
* [Coding with two qubits](#coding-with-two-qubits)
  * [Quantum entanglement](#quantum-entanglement)
  * [Nonlocal games](#nonlocal-games)
  * [Superdense coding](#superdense-coding)
* [Reference: Quantum logic gates](#reference-quantum-logic-gates)
  * [Measurement](#measurement)
  * [Single-input gates](#single-input-gates)

## Introduction
Quantum mechanics is an extraordinarily strange, unintuitive yet increasingly accurate description of how reality works at the lowest of levels. The idea that we can take this theory and use it to build an entirely new form of computing is now well-known in popular science, but to study it requires a drastic entry barrier of maths and physics. As a computer scientist/programmer, is it at all possible as of yet to break into the field of quantum programming?

The answer is **yes**. Some basic familiarty with linear algebra (read: do you know what vectors and matrices are?) would do you well, but even without this, let's take a hands-on, no-knowledge-assumed jump into quantum programming using this simulator.

This tutorial contains all examples in both C++ and the C# equivalent. Later examples may omit C# equivalents for sanity, because they are identical outside of syntax. To use Qlay in C++, link against Qlay.lib and include Qlay.h, making sure to have Qlay.dll available. To use Qlay in C#, reference both Qlay.dll and QlayCLI.dll &ndash; as the former is unmanaged, you should (in Visual Studio) manually add it as a project file and set it to copy to output in its properties.

The CLI interface is almost identical, except that standalone functions are contained in the `Core` static class, with the exception of logic gates, which are contained in `Gates`.

## The quantum bit

The basic unit of classical computing is the *bit* (binary digit). A bit can take either of the values 0 or 1. It cannot take any other values and must take one of those values at any given time.

Let's begin by treating 0 and 1 as vectors. We'll define them as:

![basisvectors.png](images/maths/basisvectors.png)

The pipe-and-chevron notation is known as a *ket vector*: for our purposes, it simply means a column vector.

0 and 1 are our only two possible states, so |0> and |1> are our *basis vectors*, or *basis states*. We can represent a single bit, *b*, using this system like so:

![linearcombination.png](images/maths/linearcombination.png)

For a classical bit, either *&alpha;* is 1 and *&beta;* is 0, or vice versa: the state is either entirely |0> or entirely |1>.

Simulating our classical bit like this allows us to extend our line of thinking a bit more: what if the state *isn't* entirely |0> or |1>, but somewhere in between? This is the motivation behind the *qubit*: the quantum bit.

A qubit can take almost any values for the coefficients *&alpha;* and *&beta;*, creating a mix of states, not quite |0> yet not quite |1>, a blur between the two. The squared magnitude of the coefficient gives the state's probability, i.e. |*&alpha;*|<sup>2</sup> is the chance it will be |0> and |*&beta;*|<sup>2</sup> is the chance it will be |1>. Therefore, these two must sum to 1, as there are no other possibilities.

## Coding with one qubit
The maths checks out, but how can a qubit possibly be probabilistic in reality? Let's set that aside for one moment: enough with the linear algebra, ahead with the coding.

### Setting up an experiment
First, we'll set up a bareboned Qlay program:

**C++:**
```c++
#include "Qlay.h"
#include <iostream>

using namespace qlay;

int main()
{
    qlay::init();

    return 0;
}
```
**C#:**
```csharp
using qlay.cli;

namespace QlayTest
{
    class Program
    {
        static void Main(string[] args)
        {
            Core.init();
        }
    }
}
```

We must call `init()` to seed the pseudorandom engine (you can provide a seed as an argument, but leaving it blank defaults to using the current time).

Inside here, let's create a qubit:

**C++:**
```c++
QubitSystem qs;
Qubit q(qs);
std::cout << M(q) << std::endl;
```
**C#:**
```csharp
//Managed Qubit is IDisposable
using (QubitSystem qs = new QubitSystem())
using (Qubit q = new Qubit(qs))
{
    bool result = Gates.M(q);

    //Render as 0 or 1
    Console.WriteLine(result ? 1 : 0);
}
```

We first instantiate a qubit system, which all qubits must be instantiated in relation to. Our qubit data type does not possess its own value in an accessible way. Instead, we use the `M()` function to measure the qubit, which returns the value as a boolean (true/false, representing |1> and |0>).

The output will be 0: all qubits are initialised into the |0> state.

As mentioned before, this system will reveal itself to be probabilistic. Rather than just doing our experiments once, we should repeat them for statistical results:

**C++:**
```c++
int repeats = 1000;
int zeroes = 0, ones = 0;

for (int i = 0; i < repeats; i++)
{
    QubitSystem qs;
    Qubit q(qs);

    M(q) ? ones++ : zeroes++;
}

std::cout << "ZERO: " << zeroes << std::endl << "ONE:  " << ones << std::endl;
```
**C#:**
```csharp
int repeats = 1000;
int zeroes = 0, ones = 0;

for (int i = 0; i < repeats; i++)
{
    using (QubitSystem qs = new QubitSystem())
    using (Qubit q = new Qubit(qs))
    {
        if (Gates.M(q)) ones++;
        else zeroes++;
    }
}

Console.WriteLine("ZERO: " + zeroes);
Console.WriteLine("ONE:  " + ones);
```
**Output:**
```
ZERO: 1000
ONE:  0
```

We repeat the experiment some suitably large number of times, counting every time the result was 0 and every time it was 1. As expected, it's 0 every time.

### Quantum logic gates
The classical bit doesn't do much by itself: we create programs by manipulating them with logic gates, such as `NOT`, `AND`, `OR` etc. We manipulate the qubit using logic gates, too, but they work rather differently indeed.

The first gate to use is called the *Pauli X gate*. It's a complicated name for a very simple function: it maps |0> to |1> and |1> to |0>. In other words, it's a direct equivalent of the classical `NOT`, or *bit-flip*.

Let's add the `X` gate to our experiment (see [PauliX.cpp](../QlayExamples/PauliX.cpp)):

**C++:**
```c++
QubitSystem qs;
Qubit q(qs);
X(q);

M(q) ? ones++ : zeroes++;
```
**C#:**
```csharp
using (QubitSystem qs = new QubitSystem())
using (Qubit q = new Qubit(qs))
{
    Gates.X(q);

    if (Gates.M(q)) ones++;
    else zeroes++;
}
```
**Output:**
```
ZERO: 0
ONE:  1000
```

Every single qubit prepared to |0> is |1>. Again, very normal.

A logic gate is actually an operation, represented by a matrix, applied to the qubit vector representation. The matrix for the Pauli X gate is:

![PauliXgateequals.png](images/maths/PauliXgateequals.png)

Matrix representations will no longer be given here, but are all [provided below](#reference--quantum-logic-gates) for each gate.

Measuring and flipping are the only things we can do with only one bit, classically, but there actually exist numerous quantum logic gates with only one input which don't have classical equivalents. For instance, *Pauli Y* and *Pauli Z* gates also exist, but their effects are subtle and not of interest to us yet.

Instead, the next essential gate to know is the *Hadamard gate*. The easiest way to understand what this gate does is that it performs half a bit-flip. If the input is |0> or |1>, the output will be exactly half-way between |0> and |1>. 

What does this mean in practice? Let's try it out in the same way as before:

**C++:**
```c++
QubitSystem qs;
Qubit q(qs);
H(q);

M(q) ? ones++ : zeroes++;
```
**C#:**
```csharp
using (QubitSystem qs = new QubitSystem())
using (Qubit q = new Qubit(qs))
{
    Gates.H(q);

    if (Gates.M(q)) ones++;
    else zeroes++;
}
```
**Output:**
```
ZERO: 497
ONE:  503
```

With the qubit set to exactly half-way between |0> and |1>, it has a 50:50 chance of being measured as 0 or 1! This is, clearly, a huge deviation from classical computing and the fundamental motivation behind the qubit, as explained earlier.

The fact that the qubit can exist as as any mixture of the two states is known as ***quantum superposition***. The qubit exists as a *superpostion* of the |0> and |1> basis states. When we observe/measure the qubit, only then does it present itself as one of those basis states, with some probability.

It goes further than even that. Let's extend our experiment so that we simply measure the qubit twice, and also count the number of times both measurements matched (see [Hadamard.cpp](../QlayExamples/Hadamard.cpp)):

**C++:**
```c++
QubitSystem qs;
Qubit q(qs);
H(q);

//Basis type is just an alias of bool
Basis result = M(q);
result ? ones++ : zeroes++;

if (M(q) == result)
    matches++;
```
**C#:**
```csharp
using (QubitSystem qs = new QubitSystem())
using (Qubit q = new Qubit(qs))
{
    Gates.H(q);

    bool result = Gates.M(q);
    if (result) ones++;
    else zeroes++;

    if (Gates.M(q) == result)
    matches++;
}
```
**Output:**
```
ZERO:  536
ONE:   464
MATCH: 1000
```

If the qubit simply had a 50:50 chance of being |0> or |1> when observed, then the number of matches would be around 50% of the time. However, we instead see that the measurements matched 100% of the time.

This means that not only did we get a 50:50 result when first observing the qubit, but the *act of measurement itself* permanently collapsed the qubit into that state. The superpostion has been destroyed and we will now measure the same result every time. This phenomenon is called ***wavefunction collapse*** and goes alongside superposition itself as perhaps the most startling, fundamental of features of quantum mechanics, entering popular science as the *Schr&ouml;dinger's cat paradox*.

### Intricacies of Pauli X and Hadamard
The description of the Hadamard gate's effects as 'half a bit-flip' is a gross simplification, albeit a useful one, but it is hard to understand it more without delving into the underlying linear algebra.

Instead, we will simply note another important property. If you were to apply 'half a bit-flip' twice, you would reasonably expect to get a full bit-flip. Is using two `H` gates equivalent to an `X` gate? Let's find out:

**C++:**
```c++
QubitSystem qs;
Qubit q(qs);
H(q);
H(q);

M(q) ? ones++ : zeroes++;
```
**C#:**
```csharp
using (QubitSystem qs = new QubitSystem())
using (Qubit q = new Qubit(qs))
{
    Gates.H(q);
    Gates.H(q);

    if (Gates.M(q)) ones++;
    else zeroes++;
}
```
**Output:**
```
ZERO: 1000
ONE:  0
```

The answer is **no**. Applying the `H` gate twice gets you right back to where you started, i.e. equals the *identity operation*. The same is, more intuitively, true of the Pauli X (`NOT`) gate (and also Y and Z, for future reference).

Speaking of the `X` gate, it is generalised to flipping all superpositions. In other words, if you had for example a qubit with a 25% chance of |0> and a 75% chance of |1>, applying the `X` gate would yield a 75% chance of |0> and 25% chance of |1> (it simply swaps the values of *&alpha;* and *&beta;*).

Finally, it is important to appreciate that a qubit can be treated as a direction vector in 3-dimensional Euclidian space, i.e. as a sphere of which the surface area maps the 2-dimensional Hilbert space (*Bloch sphere*). The Z-axis is treated by convention as the computational basis -- a qubit pointing directly along the Z-axis is |0>, whilst a qubit pointing directly against the Z-axis is |1>. One way of preparing a qubit with some probability *p* of |1> is by applying a Y-axis rotation to the default |0> qubit of 2*arcsin(sqrt(*p*)). Try deriving this from the operator matrix for the `Ry` gate in the reference section below!

## Coding with two qubits
We've seen that superposition is a key aspect of qubits. Although we can do more with a qubit than a classical bit, we still can't exactly do much with just one. The true power of quantum computing only becomes clear when we move onto using more than one qubit and the other key aspect of their behaviour: ***entanglement***.

### Quantum entanglement
First, let's look at probably the most straightforward of the logic gates which take two qubits as inputs: the `SWAP` gate.

**C++:**
```c++
QubitSystem qs;
Qubit q0(qs); Qubit q1(qs);
X(q0);
std::cout << "q0: " << M(q0) << std::endl;
std::cout << "q1: " << M(q1) << std::endl;

SWAP(q0, q1);
std::cout << "q0: " << M(q0) << std::endl;
std::cout << "q1: " << M(q1) << std::endl;
```
**C#:**
```csharp
using (QubitSystem qs = new QubitSystem())
using (Qubit q0 = new Qubit(qs))
using (Qubit q1 = new Qubit(qs))
{
    Gates.X(q0);
    Console.WriteLine("q0: " + (Gates.M(q0) ? 1 : 0));
    Console.WriteLine("q1: " + (Gates.M(q1) ? 1 : 0));

    Gates.SWAP(q0, q1);
    Console.WriteLine("q0: " + (Gates.M(q0) ? 1 : 0));
    Console.WriteLine("q1: " + (Gates.M(q1) ? 1 : 0));
}
```
**Output:**
```
q0: 1
q1: 0
q0: 0
q1: 1
```

This above example instantiates two qubits, *q<sub>0</sub>* and *q<sub>1</sub>*, and flips *q<sub>0</sub>* to |1>. This is confirmed by measurement. The `SWAP` gate is then applied, which does exactly what it says on the tin: the two qubits are swapped with each other in their entirety. Following this, *q<sub>0</sub>* now measures |0> and *q<sub>1</sub>* now |1>. The values for *&alpha;* and *&beta;* for *q<sub>0</sub>* are swapped in-place with those for *q<sub>1</sub>*.

The `SWAP` gate is of fundamental importance for low-level implementations, but we'll turn to a more interesting gate for the behaviour of interacting qubits. The controlled-NOT (`CNOT`) gate applies the `X` gate to a target qubit if an only if a control qubit measures |1> &ndash; otherwise, no change occurs. The truth table for a two-qubit system |*control*,*target*> is as follows:

| before | after |
|:------:|:-----:|
| \|00> | \|00> |
| \|01> | \|01> |
| \|10> | \|11> |
| \|11> | \|10> |

The control qubit is never affected, and the target qubit is only affected conditionally on the control. We can use this gate to demonstrate logical interaction between qubits (see [entanglement.cpp](../QlayExamples/entanglement.cpp)):

**C++:**
```c++
QubitSystem qs;
Qubit q0(qs); Qubit q1(qs);
H(q0);
CNOT(q0, q1);

Basis result = M(q0);
result ? ones++ : zeroes++;

if (M(q1) == result)
    matches++;
```
**C#:**
```csharp
using (QubitSystem qs = new QubitSystem())
using (Qubit q0 = new Qubit(qs))
using (Qubit q1 = new Qubit(qs))
{
    Gates.H(q0);
    Gates.CNOT(q0, q1);

    bool result = Gates.M(q0);
    if (result) ones++;
    else zeroes++;

    if (Gates.M(q1) == result)
        matches++;
}
```
**Output:**
```
ZERO:  502
ONE:   498
MATCH: 1000
```

First, we apply the Hadamard gate to *q<sub>0</sub>*, so it is in an equal superposition of |0> and |1>. Then, we use it as the control to `CNOT` on *q<sub>1</sub>*. Therefore, *q<sub>1</sub>* now also has a 50:50 probability between |0> and |1>. Critically, we count the number of times the measurement of *q<sub>0</sub>* equals *q<sub>1</sub>* and obtain 100%.

If *q<sub>1</sub>* simply also had a 50:50 probability then we would expect it to only match approximately 50% of the time. The true situation is that the very act of measuring and thus collapsing *q<sub>0</sub>* collapses *q<sub>1</sub>* before it is itself measured. There is seemingly 'spooky action at a distance': the two qubits interacted at some point in the past (`CNOT`) and they are now *entangled*, such that measuring one has a very real effect upon the other.

When qubits are entangled, they cannot simply be considered as independent, 2-dimensional vectors. Instead, the entire system is a 4-dimensional vector space; a linear combination of all possible states, which are |00>, |01, |10> and |11> as given in the truth table above. In this case, the *entire system* is in an equal superposition of |00> and |11>, and measuring one qubit collapses the system as a whole.

This simple example, though the most common and standard way of entagling two qubits, does not actually appear to transcend classical conditional probability, however. The true power lies in quantum algorithms and communication which exploits entanglement in ways which cannot be replicated or explained classically.

### Nonlocal games
Quantum nonlocality, the idea that states are not just subject to local realism, can be demonstrated through *nonlocal games*, experiments designed to exploit entanglement and prove quantum 'superiority'.

Consider a referee in a game and two co-operative players, *Alice* and *Bob*. The game consists of Alice and Bob each receiving a 'question' from the referee, which is just a classical bit 0 or 1, and them responding with an 'answer', likewise 0 or 1. They win the game if and only if their answers satisfy some logical predicate with the questions. The kicker is that they *cannot communicate* with each other, outside of agreeing upon a strategy beforehand.

The CHSH game consists of Alice and Bob receiving the questions *r* and *s* respectively, where both are 0 or 1 with 50:50 chance. They win if their answers *a* and *b* satisfy the predicate *r* AND *s* = *a* XOR *b*.

In other words, if *r* and *s* are both 1 then *a* and *b* must be different; otherwise, they must be the same. These winning conditions can be visualised in a truth table:

| *r* AND *s* | *a* XOR *b* |
|:---:|:---:|
| 0 AND 0 | 0 |
| 0 AND 1 | 0 |
| 1 AND 0 | 0 |
| 1 AND 1 | 1 |

Classically, the best possible strategy is trivial to determine. If Alice and Bob both give the same answers, they have a 3-in-4 chance of winning: they only lose in the 1 AND 1 case. So, Alice and Bob decide to both give the answer 0 and cannot win more assuredly than this.

However, if Alice and Bob each possess entangled qubits, they can 'cheat' the system, being able to communicate with each other to an extent despite no classical channel existing between them.

The trick is to perform measurement on the qubit(s) in particular bases. Normal measurement so far has been respect to the Z-axis: the computational basis. One can measure a qubit in any axis you want, which collapses the qubit onto that basis. We sometimes call the X-axis basis the *sign basis*. After applying Hadamard gate to |0>, you cannot say whether it will collapse to |0> or |1>, but you can say for certainty that it is in the |+> state. Likewise, a qubit prepared as |0> has a 50:50 chance of being measured to be |+> or |->, by instead measuring in the X-axis.

An `Mx` function is provided as shorthand for measuring in the X-axis, but you can measure in any arbitrary axis *A* by applying rotation gates that map *A* onto the Z-axis, and then just measuring in the Z-axis with `M`.

Alice and Bob would ideally like to communicate the question they received to the other. The best they can achieve is to measure their entangled qubits in different axes depending on the question they received, in such a way that the results (taken as their answers *a* and *b*) have the highest probability of falling into a winning condition. The derivation of the optimal strategy and axes to measure in is beyond this scope, but see [CHSH.cpp](../QlayExamples/CHSH.cpp) for it implemented and performed. The optimal strategy is that Alice measures in the X axis if *r* is 1 and in the Z axis otherwise, and Bob measures in the same but rotated &pi;/8 radians. The results as shown are conclusive: the classical strategy's success rate is 75%, but the quantum strategy's is ~85%!

The GHZ game (see [GHZ.cpp](../QlayExamples/GHZ.cpp)) is very similar, but introduces a third player, *Charlie*, changes the predicate and restricts the possible set of questions they receive. The best classical success rate is again 75%. By sharing three entangled qubits prepared to a state correspending to the possible set of questions, a simpler quantum strategy of applying the Hadamard gate once achieves something spectacular: a 100% success rate.

These games prove that quantum entanglement transcends mere conditional probability: that entangled states cannot be considered in isolation and can be dependent even when physically separated.

### Superdense coding
Entanglement allows for extremely powerful techniques in communication, of which there's two major ones we'll look at. The first is ***superdense coding***, wherein we can transmit two classical bits of information by transmitting only one qubit.

We mentioned earlier that the most basic way of creating an entangled state is by applying a `H` gate followed by a `CNOT`. This actually creates one of the four most simple and maximally entangled states possible between two qubits: one of the four *Bell states*. These are as follows:

![Bellstates.png](images/maths/Bellstates.png)

Applying `H` then `CNOT` creates the first Bell state (|&Phi;<sup>+</sup>>). The two &Phi; states are where the two qubits are equal and the two &Psi; states are where they are different. Between each of the states of these types, the superscript sign indicates the phase factor. Remember, this phase factor does not directly affect the probability amplitudes, but are distinguishable when applying other operations.

The problem at hand is that Alice wishes to send two classical bits of information to Bob, by transmitting only one qubit. A simple insight is that there are four possible states she could wish to send: 00, 01, 10 or 11. The overall idea is thus straightforward: if Alice and Bob share an entangled state, Alice can transform the Bell state (encoding her message) such that Bob can determine the transformation made (decoding her message).

First, Alice and Bob receive a qubit each, consituting the first Bell state (|&Phi;<sup>+</sup>>):

**C++:**
```c++
QubitSystem qs;
Qubit qa(qs); Qubit qb(qs);

//Prepare Alice's and Bob's qubits in Bell state |Φ+>
H(qa);
CNOT(qa, qb);

//Bob 'undoes' the entanglement
CNOT(qa, qb);
H(qa);

std::cout << "Message received: " << M(qa) << M(qb) << std::endl;
```
**Output:**
```
Message received: 00
```

So far, Alice receives *q<sub>a</sub>* and Bob receives *q<sub>b</sub>*. Alice currently does nothing and sends *q<sub>a</sub>* to Bob. Bob decodes by applying `CNOT` then `H`, 'undoing' the entanglement. It is clear that Bob just returns the state to its initial preparation (|00>) by symmetry, because you should remember that all gates are reversible.

If Alice intends to transmit the message 00, then this is already achieved. If she intends to transmit a different message, then she should transform the state into a different Bell state, by applying a certain operation(s) to *q<sub>a</sub>* before sending it, as below:

| Message | Target state | Operation(s) |
|:-------:|:------------:|:------------:|
| 00 | \|&Phi;<sup>+</sup>> | none |
| 01 | \|&Psi;<sup>+</sup>> | `X` |
| 10 | \|&Phi;<sup>-</sup>> | `Z` |
| 11 | \|&Psi;<sup>-</sup>> | `X` then `Z` |

We can try each of these cases to confirm they send the intended message correctly (see [superdense_coding.cpp](../QlayExamples/superdense_coding.cpp)):

**C++:**
```c++
QubitSystem qs;
Qubit qa(qs); Qubit qb(qs);

//Prepare Alice's and Bob's qubits in Bell state |Φ+>
H(qa);
CNOT(qa, qb);

//If sending 00, do nothing

//If sending 01, transform into |Ψ+>
//X(qa);

//If sending 10, transform into |Φ->
//Z(qa);

//If sending 11, transform into |Ψ->
//X(qa);
//Z(qa);

//Bob 'undoes' the entanglement
CNOT(qa, qb);
H(qa);

std::cout << "Message received: " << M(qa) << M(qb) << std::endl;
```

When working through examples and trying to understand the motivations and reasonings, it can be most useful to view the entire quantum state at specific points as a column vector. The `QubitSystem` object can just be streamed to standard output. This is, naturally, a debugging feature as a benefit of a simulation: the key principle is that true quantum systems collapse as a result of observation.

## Reference: Quantum logic gates
This section outlines all of the quantum logic gates, explaning them by their operator matrices and effects on a qubit by treating it as a spin state &ndash; understanding their intricacies is not necessarily crucial to start quantum programming. All angles are given in radians.

### Measurement
| Function header | Description |
|:---------------:| ----------- |
| `M(q)` | The 'normal' measurement, returning 0 or 1. Measures the qubit in the Z-axis, i.e. the computational basis.
| `Mx(q)` | Measures the qubit in the X-axis, i.e. the sign basis.

### Single-input gates
| Gate | Function header | Operator matrix | Description |
|:----:|:---------------:| --------------- | ----------- |
| Pauli X | `X(q)` | ![PauliXgate.png](images/maths/PauliXgate.png) | Bit-flip, equivalent to classical `NOT`. Performs a rotation of *&pi;* around the X-axis. Maps \|0> to \|1> and \|1> to \|0>.
| Pauli Y | `Y(q)` | ![PauliYgate.png](images/maths/PauliYgate.png) | Performs a rotation of *&pi;* around the Y-axis. Maps \|0> to *i*\|1> and \|1> to &minus;*i*\|0>.
| Pauli Z | `Z(q)` | ![PauliZgate.png](images/maths/PauliZgate.png) | Phase-flip. Performs a rotation of *&pi;* around the Z-axis. Leaves \|0> unchanged and maps \|1> to &minus;\|1>.
| Hadamard | `H(q)` | ![Hadamardgate.png](images/maths/Hadamardgate.png) | Creates a superposition by mapping \|0> to (\|0>&plus;\|1>)/sqrt(2) and \|1> to (\|0>&minus;\|1>)/sqrt(2). Performs a rotation of *&pi;* around the X-axis followed by *&pi;*/2 around the Y-axis.
| Square root NOT | `SRNOT(q)` | ![SRNOTgate.png](images/maths/SRNOTgate.png) | Square root of the Pauli X gate. Appling two in sequence is thus equivalent to a single `NOT`.
| X rotation | `Rx(angle, q)` | ![Rxgate.png](images/maths/Rxgate.png) | Rotates around the X-axis by the given angle.
| Y rotation | `Ry(angle, q)` | ![Rygate.png](images/maths/Rygate.png) | Rotates around the Y-axis by the given angle.
| Z rotation | `Rz(angle, q)` | ![Rzgate.png](images/maths/Rzgate.png) | Rotates around the Z-axis by the given angle.
| Phase shift | `Rp(angle, q)` | ![phaseshiftgate.png](images/maths/phaseshiftgate.png) | Performs a phase shift by the given angle, mapping \|1> to exp(*i&theta;*)\|1>.
