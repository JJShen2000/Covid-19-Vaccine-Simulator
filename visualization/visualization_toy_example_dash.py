import dash
from dash import dcc
from dash import html
from dash.dependencies import Input, Output
import plotly.express as px
import csv
import pandas as pd
import numpy as np

# original code: https://dash.gallery/python-docs-dash-snippets/choropleth-maps

# token
# token = open(".mapbox_token").read()

def create_toy_df():
    ''' Create the toy data.
    '''
    regions = []
    with open('census_tracts.csv', newline='', encoding="utf-8") as csvfile:
        spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
        for row in spamreader:
            regions.append(str(row[0]).replace('臺', '台')+'/'+str(row[1]))

    df = pd.DataFrame(regions, columns=['REG'])
    new_df = df.copy()
    new_df2 = df.copy()
    df['Time'] = np.ones((368,), dtype=int)
    new_df['Time'] = np.full((368, ), 2)
    new_df2['Time'] = np.full((368, ), 3)
    df = pd.concat([df, new_df, new_df2])
    df['I'] = np.random.randint(0,30,size=368*3)
    df['D'] = np.random.randint(0,5,size=368*3)
    return df


df = create_toy_df()
classes = ['I', 'D']

app = dash.Dash(__name__)

app.layout = html.Div([
    html.P("Choose the data you want to know:"),
    dcc.RadioItems(
        id='candidate', 
        options=[{'value': x, 'label': x} for x in classes],
        value=classes[0],
        labelStyle={'display': 'inline-block'}
    ),
    dcc.Graph(id="choropleth"),
])

@app.callback(
    Output("choropleth", "figure"), 
    [Input("candidate", "value")])
def display_choropleth(candidate):
    fig = px.choropleth_mapbox(data_frame=df, 
        geojson='https://raw.githubusercontent.com/JJShen2000/Covid-19-Vaccine-Simulator/main/src/twTown.geo.json', 
        locations='REG', 
        featureidkey='properties.name',  
        color=candidate,
        opacity=0.7,
        color_continuous_scale=px.colors.sequential.Reds,
        range_color=([30, 0] if candidate=='I' else [0, 10]),
        animation_frame='Time',
        title='Taiwan Covid-19',
        height=800,
        width=800,
        mapbox_style="open-street-map",
        center={"lat": 24, "lon": 120},
        zoom=6
        )
    fig.update_geos(showcountries=False, showcoastlines=False, showland=False, fitbounds="locations")
    fig.update_layout(margin={"r":0,"t":0,"l":0,"b":0})

    return fig

app.run_server(debug=True)
