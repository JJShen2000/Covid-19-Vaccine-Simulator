import dash
from dash import dcc
from dash import html
import dash_bootstrap_components as dbc
from dash.dependencies import Input, Output, State
import plotly.express as px
import pandas as pd
import os

import views.line_chart
import models.data_line_chart
import views.choropleth
import models.data_choropleth


# Init. of app
app = dash.Dash(__name__, external_stylesheets=[dbc.themes.BOOTSTRAP])
app.title = 'Covid-19'
server = app.server

# SIDEBAR_STYLE = {
#     "position": "fixed",
#     "top": 62.5,
#     "left": 0,
#     "bottom": 0,
#     "width": "16rem",
#     "height": "100px",
#     "z-index": 1,
#     "overflow-x": "hidden",
#     "transition": "all 0.5s",
#     "padding": "0.5rem 1rem",
#     "background-color": "#f8f9fa",
# }

app.layout = html.Div(
    children=[
        dcc.Location(id='url'),
        html.Div(
            className="top_bar",
            children=[html.H1('Covid-19', style={'margin': '0.5em auto'})]
        ),
        html.Div(
            className="left_menu",
            children=[
                dbc.Nav(
                    [
                        dbc.NavItem(dbc.NavLink('Front Page', href='/', active="exact")),
                        dbc.NavItem(dbc.NavLink('Choropleth', href='/choropleth', active="exact")),
                        dbc.NavItem(dbc.NavLink('Line Chart', href='/line_chart', active="exact"))
                    ],
                    pills=True,
                    vertical=True
                    # fill=True,
                )
            ],
            # style=SIDEBAR_STYLE
        ),
        html.Div(id='page-content')
    ]
)

@app.callback(
    Output("choropleth1", "figure"), 
    [Input("candidate", "value")],
    [Input("scenario_dropdown1", "value")],Input('my-slider', 'value'))
def display_choropleth1(candidate, scenario_dropdown, slider_val):
    # data_frame = df if scenario_dropdown==1 else df_another
    data_frame = models.data_choropleth.dict_df_by_period[scenario_dropdown][slider_val-1]

    fig = px.choropleth_mapbox(data_frame=data_frame , 
        geojson='https://raw.githubusercontent.com/JJShen2000/Covid-19-Vaccine-Simulator/main/src/visualization/twTown.geo.json', 
        locations='Town', 
        featureidkey='properties.name',  
        color=candidate,
        opacity=0.7,
        color_continuous_scale=px.colors.sequential.Reds,
        range_color=(models.data_choropleth.range_color_classes[candidate]),
        animation_frame='Period',
        height=600,
        width=700,
        mapbox_style="open-street-map",
        center={"lat": 24, "lon": 120},
        zoom=6
        )
    fig.update_geos(showcountries=False, showcoastlines=False, showland=False, fitbounds="locations")
    fig.update_layout(margin={"r":0,"t":0,"l":0,"b":0})
    if candidate[0] == 'n':
        fig.update_coloraxes()

    return fig


@app.callback(
    Output("choropleth2", "figure"), 
    [Input("candidate", "value")],
    [Input("scenario_dropdown2", "value")],Input('my-slider', 'value'))
def display_choropleth2(candidate, scenario_dropdown, slider_val):
    # data_frame = models.data_choropleth.Data_choropleth.df if scenario_dropdown==1 else models.data_choropleth.Data_choropleth.df_another
    # data_frame = data_frame[data_frame['Period']==slider_val ]
    data_frame = models.data_choropleth.dict_df_by_period[scenario_dropdown][slider_val-1]
    fig = px.choropleth_mapbox(data_frame=data_frame , 
        geojson='https://raw.githubusercontent.com/JJShen2000/Covid-19-Vaccine-Simulator/main/src/visualization/twTown.geo.json', 
        locations='Town', 
        featureidkey='properties.name',  
        color=candidate,
        opacity=0.7,
        color_continuous_scale=px.colors.sequential.Reds,
        range_color=(models.data_choropleth.range_color_classes[candidate]),
        # animation_frame='Period',
        height=600,
        width=700,
        mapbox_style="open-street-map",
        center={"lat": 24, "lon": 120},
        zoom=6
        )
    fig.update_geos(showcountries=False, showcoastlines=False, showland=False, fitbounds="locations")
    fig.update_layout(margin={"r":0,"t":0,"l":0,"b":0})

    return fig


@app.callback(
    Output("line-chart", "figure"), 
    [Input("line-chart-data-class", "value")],[Input('line-chart-country', 'value')], [Input('line-chart-town', 'value')])
def update_line_chart(data_class, country, town):
    mask = ((models.data_line_chart.Data_line_chart.df['Town']==town) & (models.data_line_chart.Data_line_chart.df['Country']==country))
    fig = px.line(models.data_line_chart.Data_line_chart.df[mask], 
        x="Period", y=data_class, color='scenario',height=500)
    fig.update_layout(
        legend_title="Scenario",
        font=dict(
            size=18
        )
    )
    return fig


@app.callback(
    dash.dependencies.Output('my-slider', 'value'),
    [dash.dependencies.Input('auto-stepper', 'n_intervals')],
    )
def on_click(n_intervals):
    if n_intervals is None:
        return 0
    else:
        return (n_intervals+1)%models.data_choropleth.period_num


@app.callback(
    Output("auto-stepper", "disabled"),
    Output('auto-stepper', 'n_intervals'),
    Input("play", "n_clicks"),
    State("auto-stepper", "disabled"),
    State('my-slider', 'value')
)
def toggle(n, playing, slider_val):
    if n:
        if playing:
            return (not playing), slider_val  
        return (not playing), dash.no_update 
    return playing, dash.no_update 
    # return playing


# Update dropdown of line chart
@app.callback(
    dash.dependencies.Output('line-chart-town', 'options'),
    [dash.dependencies.Input('line-chart-country', 'value')]
)
def update_dropdown(name):
    return [{'label': i, 'value': i} for i in models.data_line_chart.tract_option_dict[name]]

@app.callback(
    dash.dependencies.Output('line-chart-town', 'value'),
    [dash.dependencies.Input('line-chart-country', 'value')]
)
def update_dropdown(name):
    return models.data_line_chart.tract_option_dict[name][0]


@app.callback(
    Output('page-content', 'children'),
    Input('url', 'pathname')
)
def render_page_content(pathname):
    if pathname == '/' or pathname == '':
        return html.Div(className="content", children=html.H1('This is the visualization of Covid-19 simulator!'))
    elif pathname == '/choropleth':
        return views.choropleth.choropleth_page
    elif pathname == '/line_chart':
        return views.line_chart.line_chart_page

    return html.Div(className="content", children=html.H1('404！'))


if __name__ == '__main__':
    app.run_server()