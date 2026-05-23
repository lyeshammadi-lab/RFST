# KrakenSDR DoA — Temps réel

Moteur DSP adaptatif (GSC) pour estimation de direction d'arrivée
avec la KrakenSDR (5 canaux cohérents).

## Stack
- Acquisition : HeIMDALL DAQ Firmware (Linux)
- Cœur DSP   : C++17 + Eigen + pybind11
- Interface  : PyQt6 + pyqtgraph

## Setup
```bash
pip install -r requirements.txt
pip install -e . --no-build-isolation
python gui/main_window.py
```
