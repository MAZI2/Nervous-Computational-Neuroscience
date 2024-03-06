# Project nervous
Nervous is a project for researching brain functions through abstraction and simplification of biological processes that enable information flow and learning in brain. It's goal is to be a gradually developing system designed in such a way, that new creative ideas in brain research can be quickly implemented and explored in a system that resembles human brain.

You are welcome to [reach me](#about) if you share similar interests!

Build the project [ here](#building-the-project).

The project is currently written in C, but will probably be rewritten in C++. The code and algorithms can be written way faster, better and more organised.


![ezgif com-video-to-gif](https://github.com/MAZI2/Nervous-Computational-Neuroscience/assets/65289043/ccf9cf00-f687-4e8a-9ab8-d5ed190ef1e2)

## Project idea
Nervous is a framework for visualizing and exploring neural networks that behave as similarly as possible to the human brain. It is not intended to reach a certain goal, rather to support quickly exploring ideas and hypotheses on neuron scale brain functions. This project aims to explore the autonomous organization of neurons and brain functions that emerge.

The project was currently used to explore:

- [Association between two paths](https://drive.google.com/file/d/1XTYDKF0OEK_ByyZHTQKWOlLMf3-tvzsJ/view?usp=drive_link) (network is trained for 300 repetitions with firing a secondary neuron, after that the input is associated with the output on its own)
- Effects of dopamine on the neural network and ideas of its involvement in decision making and learning
- Pretrained paths (from biolog. genetically generated) and competition & dopamine effect on such decision making

[Go to research](#current-and-further-research)

The framework supports visualization through [GLFW(OpenGL)](https://github.com/glfw/glfw) and simple statistical plotting using [pbPlots](https://github.com/InductiveComputerScience/pbPlots)

## Architecture
- the network is updated synchronously. Neurons that received signals from previous iteration get incremented or decreased potential (excitatory/inhibitory) and fire if above threshold. 
- neurons are inhibitory / excitatory in 1:2 ratio
- for research of dopamine effects on network, neurons have excitatory / inhibitory response (receptors) to dopamine in 1:1 ratio (D1-5 receptor classes). [Other neurotransmitters](#topics-for-research) can be added similarly 
- connections get strengthened or weakened in hebbian fashion. If a neuron that fired successfuly caused a neuron to fire in next iteration, the connection is strengthened. If a neuron fired but was unsuccessful, the connection weakens. 
- Connections that are not involved in iteration are currently unchanged.

**this is just a core system that should be upgraded or configured to support your ideas in research (add your own features)**

## Current and further Research
- Paths through network are associated given enough stimuli (more succesful excitations) 
- Dopamine equaly inhibits and excites neurons in network, but because of the inhibitory/excitatory neuron type ratio, the net effect is excitatory
----
#### Observed
- If dopamine released as reward with certain decay time, the activity following the reward is encouraged or fires more successfully. The neuroplasticity is therefore temporarily incresed after the dopamine release.
- If a path in network is (artificially) strengthened, the convergence towards it is therefore increased after the dopamine release.
- Encouraging a sequence that caused rewarded output to fire also encourages following or preceding wrong output fires. [Avoidance of wrong outputs](#ideas-to-be-tested-and-explored) should be explored.
----
#### Ideas to be tested and explored
- Avoidance of negative stimuli:
    - Serotonin can be sometimes be considered as opposite of dopamine in inhibition and avoidance of action that results in negative reward.
    - Living mice brain has been trained in the past to play games through reinforcement learning ([DishBrain](https://www.cell.com/neuron/pdfExtended/S0896-6273(22)00806-6)). Correct output was followed with predictable repeating signal on all input neurons. Wrong output was discouraged with sending signals to random input neurons. System behaved in a [Free energy principle](https://en.wikipedia.org/wiki/Free_energy_principle) which is a Bayesian approach to exploring brain functions, but it is not really autonomous in the previous example.
    - Repeating/predictable following signal can be explained to be encouraging to input - output association since the same path will fire multiple times and strengthen -> higher probability of stimuli to converge to this path (more/stronger paths through brain leading to that output).
    - Random signal as discouragement of action causes different combinations to be momentarly associated with given input, promoting different responses (a way for different response than the one current - wrong one to emerge)
    - In human brain, the response is probably often not random, but already at least partially learned - associated with the situation (running away, a proccess of correcting the mistake...)

##### To Do
- On wrong output, fire random nerves. On correct output encourage action with dopamine (already implemented).
- Implement new synapses forming towards very active neurons (more opportunity for learning?)
- A more autonomous way of implementing serotonin would be without randomness and with a trained cluster for correcting mistake.

- A goal can be to play a game of pong. A mistake correcting cluster could be as simple as trying a different direction (firing other output neuron).
----
#### Topics for research
- Effects of different neurotransmitters on neuron to neuron scale.
- Glutamate seems to have important role in learning
- Serotonin
- GABA
- Appetitive and aversive events

- network behaviour varies greatly with parameter changes, so the right homeostasis supporting an idea is often hard to achieve
- recreating a certan human/animal action/decision with such network

#### Some related literature
- Baunez C, Nieoullion A, Amalric M. N-Methyl-D-aspartate receptor blockade impairs behavioural performance of rats in a reaction time task: new evidence for glutamatergic /dopaminer- gic interactions in the striatum. Neuroscience 1994;61:521 /31.

- Adriani W, Felici A, Sargolini F, Roullet P, Usiello A, Oliverio A, Mele A. N -Methyl-D -aspartate and dopamine receptor involvement in the modulation of locomotor activity and memory processes. Exp Brain Res 1998;123:52 /9.

- Amalric M, Ouagazzal A, Baunez C, Nieoullon A. Functional interactions between glutamate and dopamine in the striatum. Neurochem Int 1994;25:123 /31.

- Riedel G, Wetzel W, Reymann KG. Comparing the role of metabotropic glutamate receptors in long-term potentiation and in learning and memory. Progr Neuropsychopharmacol Biol Psychiatr 1996;20:761 /89.

- Bliss TVP, Collingridge GL. A synaptic model of memory: long- term potentiation in the hippocampus. Nature 1993;361:31 /9.

- Bear MF, Abraham WC. Long-term depression in hippocampus. Ann Rev Neurosci 1996;19:437 /62.

### About
The idea of the network was motivated by thinking about everyday thought patterns and reward seeking. The whole project is currently a fairly fresh solo project which shaped itself into an idea that I want to share with the world. If you are interested in casually contributing to the project or share similar interest in human brain as me, feel free to contribute to this Github repository or reach out to me at <matjazpogacnik5@gmail.com>

**The broader brainstorming led to ideas that inspired the project**

The consciousness might be understood as cycles (emerging from all different outputs) converging to different established paths (visualizing a thought fires the same pathway that a seight would).
"Logic" functioning of brain is also a new problem converging to pathways of similar problems being solved in the past. So the brain does not know "right" or "wrong" answer as we are more or less confident in a more right or more wrong reponse.


## Building the project
## GLFW CMake starter
GLFW homepage: [glfw.org](https://www.glfw.org/)  
GLFW on GitHub: [github.com/glfw/glfw](https://www.github.com/glfw/glfw)

```
git clone --recursive https://github.com/juliettef/GLFW-CMake-starter
```
This will create the directory _GLFW-CMake-starter_ and get the latest source code, using the ```--recursive``` option to download the GLFW code which is included in the repository as a submodule.

## Using CMake to create the project

From a command prompt in the `GLFW-CMake-starter` directory:
1. `mkdir build`
1. `cd build`
1. `cmake ..` (for MinGW the build system generator needs to be specified using the -G option: `cmake .. -G "MinGW Makefiles"`)
1. Either run `make all` or for Visual Studio open `GLFW-CMake-starter.sln` or for MinGW run `mingw32-make`

