# Fuzzy Logic Traffic Signal Optimizer 🚦

This repository contains a traffic signal cycle length optimizer that uses a **Mamdani Fuzzy Inference System** built in C++. It compares the performance of the fuzzy logic controller against the classic **Webster's Formula** (a fixed-time model) under varying conditions of traffic density and flow.

The project acts as a foundational artificial intelligence pipeline, generating simulation data in C++ and visualizing the results using Python.

## ⚙️ Features
* **Custom Fuzzy Logic Engine:** Implements triangular membership functions, fuzzification, rule evaluation (Mamdani AND), and Center of Gravity defuzzification from scratch in C++.
* **Baseline Comparison:** Benchmarks the AI controller against Webster's standard traffic engineering formula.
* **Automated Pipeline:** Uses a Windows batch script to compile the C++ code, run the simulation, and trigger a Python script to graph the results.
* **Data Visualization:** Automatically generates publication-ready graphs using `matplotlib` and `pandas`.

## 📁 Repository Structure
* `traffic_controller.cpp`: The core C++ simulation and fuzzy logic engine.
* `python.py`: The Python script that reads the generated CSV and plots the data.
* `run_simulation.bat`: The executable batch script that runs the entire pipeline from compilation to visualization.

## 🚀 How to Run

### Prerequisites
To run this project locally, you will need:
1. A C++ compiler (like `g++`) added to your system PATH.
2. Python 3.x installed.
3. The following Python libraries:
   ```bash
   pip install pandas matplotlib
