# Visualization_Covid-19-Vaccine-Simulator

## Data Preprocessing

- Input path: `data/vis_data` (If your pwd is Covid-19-Vaccine-Simulator)

- For line chart by scenario

  `python3 preprocessing/lc_scenario.py` (If your pwd is Visualization_Covid-19-Vaccine-Simulator)

- For line chart by state

  `python3 preprocessing/lc_state.py` (If your pwd is Visualization_Covid-19-Vaccine-Simulator)

Output of preprocessing: `preprocessing/data`

- choropleth not yet

## Run website

- If you want the data on the github,

  `python3 app.py`

- If you want the local data(`preprocessing/data`)

  `python3 app.py --local`