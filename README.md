# Project nervous
Nervous is a passion project that I am opening to public to gather ideas and people with similar interes in Computational Neurology and Brain Simulation. The beauty and efficiency of code was subordinate to the workflow of quickly adding feature on feature to support ideas in research.

The code can therefore be written way faster if/when that becomes necessary.

![](https://media.giphy.com/media/w7pXFDeuOcnQdgpR9c/giphy.gif)


## Project idea
Nervous is a framework for visualizing and exploring nural networks that behave as similarly as possible to the human brain. It is not intended to reach a certain goal, rather to support quickly exploring ideas and hypotheses on neuron scale brain functions. This project aims to explore the authonomous organization of neurons and brain functions that emerge. Not the external mathematical methods for training neural networks!

The project was currently used to explore:

- [Association between two paths](https://drive.google.com/file/d/1XTYDKF0OEK_ByyZHTQKWOlLMf3-tvzsJ/view?usp=drive_link)
- Excitation & inhibition
- Effects of dopamine on the neural network and ideas of its involvement in decision making and learning
- Pretrained paths (from biolog. genetically generated) and competition & dopamine effect on such decision making

[Go to research](#current-and-further-research)

The framework supports visualization through [GLFW(OpenGL)](https://github.com/glfw/glfw) and simple statistical plotting using [pbPlots](https://github.com/InductiveComputerScience/pbPlots)

## Architecture
- the network is updated synchronously. Neurons that received signals from previous iteration get incremented or decreased potential (excitatory/inhibitory) and fire if above threshold. 
- neurons are inhibitory / excitatory in 1:2 ratio
- neurons have excitatory response (receptors) to dopamine in 1:1 ratio
- connections get strengthened or weakened in hebbian fashion. If a neuron that fired successfuly caused a neuron to fire in next iteration, the connection is strengthened. If a neuron fired but was unsuccessful, the connection weakens. 
- Connections that are not involved are currently unchanged.

**current specifications can be configured to support other ideas in research**

## Current and further Research
- Paths through network are associated given enough stimuly (more succesful excitations) 

- Dopamine equaly inhibits and excites neurons in network, but because of the inhibitory/excitatory neuron type ratio, the net effect is excitatory, as it should be?
- If dopamine released as reward with certain decay time, the activity following the reward is encouraged/more successful. The neuroplasticity is therefore temporarily incresed after the dopamine release.
- If a path in network is (artificially) strengthened, the convergence towards it is therefore increased after the dopamine release.
- If two paralel such paths are mapped from separate inputs to separate outputs, the resulting output given different input sequences can somewhat be steered with dopamine "reward".
- With such reward, a fresh network might be able to be trained with a constant input firing and a constant output being rewarded, but currently a pattern is not consistent, as wrong outputs following the correct output are also encouraged.
- Such recurrent networks are very unpredictable and do not map single input to single output, but rather sequences to sequences. Learning should therefore not be pursued as "trying to teach a network to mimic XOR gate", as a correct output for a brain is everything that led to reward and perhaps also some following sequence. 


**all the hypotheses and observations are merely observations**

### The broader brainstorming led to ideas that inspired the project
The network idea was motivated by thinking about everyday thought patterns always leading to reward seeking.
The consciousness might also be understood as cycles (emerging from all different outputs) converging to different established paths (visualizing a thought fires the same pathway that a seight would).
"Logic" functioning of brain is also a new problem converging to pathways of similar problems being solved in the past. So the brain does not know "right" or "wrong" answer as we are more or less confident in a more right or more wrong reponse.

### What should be explored further
- implementing and researching other **neurotransmitters**
- network behaviour varies greatly with parameter changes, so the right **homeostasis** supporting an idea is often hard to achieve
- how a certain **human/animal action** can be realized with such network 

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

