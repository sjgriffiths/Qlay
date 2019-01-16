# Qlay

C++ Quantum Playground

## About
Qlay is a C++ quantum computing simulation library, allowing for the programming of small quantum logic systems with a simple, easy-to-learn interface. Qlay Visual is a GUI tool built on Qlay to build circuits visually, trading the flexibility of the code environment for an even more approachable, graphical introduction.

With the tutorial-style documentation, one only needs a basic understanding of classical logic (AND, OR etc.) in order to jump into the world of quantum programming with no prior experience.

## Components
* **Qlay:**
The core C++ API, consisting only of Qlay.dll+lib and Qlay.h.
* **QlayExamples:**
C++ projects demonstrating the use of the library and basic quantum concepts.
* **QlayCLI:**
C++/CLI wrapper library to allow the use of Qlay in managed code environments.
* **QlayVisual:**
Graphical tool for the visualisation and building of quantum logic circuits.

## Tutorials
The Qlay documentation functions as an introduction to both using the library/GUI and teaching the fundamental quantum concepts from scratch. For this latter purpose, the tutorials are largely equivalent depending on which way you would like to learn &ndash; *both* is recommended!

* [Qlay (C++/C#) tutorial](QLAY.md)
* [Qlay Visual tutorial](QLAYVISUAL.md)

## Versioning
| Version | Description | Released |
|:-------:| ----------- |:--------:|
| [v1.0.0](https://github.com/sjgriffiths/Qlay/releases/tag/v1.0.0) | The API is defined, set up as open-source and tutorial-style documentation is initiated. The library supports programming with a single qubit and single-input logic gates. | 12/10/2018
| [v1.1.0](https://github.com/sjgriffiths/Qlay/releases/tag/v1.1.0) | Qlay Visual, the circuitry GUI program, is launched using the Qlay API. | 09/11/2018
| [v2.0.0](https://github.com/sjgriffiths/Qlay/releases/tag/v2.0.0) | The API is refactored to support programming with multiple qubits and include multi-input logic gates. | 09/01/2019
| [v2.1.0](https://github.com/sjgriffiths/Qlay/releases/tag/v2.1.0) | Qlay Visual is extended with the refactored API to support multiple qubits. | 16/01/2019
| v2.2.0 | The API includes some basic abstractions for treating qubit strings as integers, further supporting arithmetical context.

## License
This API, including the GUI visualiser, is hosted open-source under the [MIT license](../LICENSE.md).

Qlay utilises the [Eigen library](https://bitbucket.org/eigen/eigen) for underlying linear algebra.
