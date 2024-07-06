
<a id="readme-top"></a>










<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>


## About The Project
![Preview](https://github.com/MassimilianoCEZ/MicroReef/assets/87046265/ac015944-b4dc-43dc-a507-e2e802e337db)

This project has been developped during the second semester at EPFL by Ferrulli Massimiliano and Carl Johan Waldorff. It consists in a 2D simulation of an aquatic microsystem written entirely in C++. The system is composed by algae (green dots), scavengers (red circles) and corals (blue segment).

### Tools

Those are the tools we used:
* C++ 11
* Gtkmm 4.0
* Makefile
* Git
* Liveshare
* Latex
<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Prerequisites

In order to use this project you have to install gtkmm 4.0 
* Linux (pacman):
  ```
  sudo pacman -S gtkmm-4.0
  ```
### Installation and compiling
1. Clone the repo
   ```
   git clone https://github.com/MassimilianoCEZ/MicroReef
   ```
2. Change directory
   ```
   cd Microreef
   ```
3. Compiling
   ```
   make 
   ```
## Usage
after compiling you can launch the project via:
   ```
   ./projet 
   ```
or you can launch it directly with an opened file (replace the XX with the actual number of the test file):
   ```
   ./projet TESTfiles/tXX.txt 
   ```
Once the GUI is opened you will find on the left 5 buttons. 
* The open button is used to initialize the simulation opening a .txt file overwriting the current one.
* The save button is used to save all the infos of the current status of the simulation, giving the user the possibility to reopen it later.
* The start button will start the simulation and once pressed becomes a stop button.
* The step button does only one update on the simulation.
On the left is also present a check box called "Birth of algae" that will let spawn the algae randomly in the world every update.
the number of updates and the number of entities for each type is updated and show in the left.

