import dash
from dash import dcc
from dash import html
import dash_bootstrap_components as dbc
from dash.dependencies import Input, Output, State
from pandas.core.algorithms import mode
import plotly.express as px
import plotly.graph_objects as go

import views.line_chart
import models.data_line_chart
import views.choropleth
import models.data_choropleth
import views.line_chart_state_page
import models.data_line_chart_state


# Init. of app
app = dash.Dash(__name__, external_stylesheets=[dbc.themes.BOOTSTRAP])
app.title = 'Covid-19'
server = app.server

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
                        dbc.NavItem(dbc.NavLink('Line Chart - By Scenarios', href='/line_chart', active="exact")),
                        dbc.NavItem(dbc.NavLink('Line Chart - By States', href='/line_chart_state', active="exact"))
                    ],
                    pills=True,
                    vertical=True
                    # fill=True,
                )
            ],
        ),
        html.Div(id='page-content')
    ]
)

def generate_choropleth(choro_id, data_class, slider_val):
    if choro_id == 1:
        data_frame = models.data_choropleth.df_sc1_by_period[slider_val-1]
    elif choro_id == 2:
        data_frame = models.data_choropleth.df_sc2_by_period[slider_val-1]

    column = data_class

    if 'ratio' in data_class:
        hover_data = [data_class, data_class[:-8]]
    elif 'in that' in data_class:
        hover_data = [data_class]
    else:
        hover_data = [data_class, data_class+' (ratio)']
        
    fig = px.choropleth_mapbox(data_frame=data_frame , 
        geojson='https://raw.githubusercontent.com/JJShen2000/Covid-19-Vaccine-Simulator/main/src/visualization/twTown.geo.json', 
        locations='Town', 
        featureidkey='properties.name',  
        color='log1p('+column+')',
        opacity=0.7,
        color_continuous_scale=px.colors.sequential.Reds,
        range_color=(models.data_choropleth.range_color_classes[column]),
        animation_frame='Period',
        height=600,
        width=700,
        mapbox_style="open-street-map",
        center={"lat": 24, "lon": 121},
        zoom=6.5,
        hover_data=hover_data
        )

    if data_class == 'Infections':
        fig.update_layout(coloraxis_colorbar=dict(
            tickmode='array',
            title=data_class,
            tickvals=[0, 2.3978952727983707, 4.61512051684126, 6.90875477931522,9.210440366976517,11.51293546492023],
            ticktext=["0", "10", "100", "1K", "10K", "100K"],
        ))
    fig.update_geos(showcountries=False, showcoastlines=False, showland=False, fitbounds="locations")
    fig.update_layout(margin={"r":0,"t":0,"l":0,"b":0})

    return fig

@app.callback(
    Output("choropleth1", "figure"), 
    [Input("choropleth-dataclass", "value")],
    Input('my-slider', 'value'),
    [Input('age_group', 'value')],
    [Input("sc1_conf", "value")],
    [Input("sc1_init_infector", "value")],
    [Input("sc1_vaccine_strategy", "value")]
)
def display_choropleth1(data_class, slider_val, age, sc_conf, sc_init_inf, sc_vac_strategy):
    models.data_choropleth.update_df_by_age_scenario(1, age, sc_conf, sc_init_inf, sc_vac_strategy)
    return generate_choropleth(1, data_class, slider_val)


@app.callback(
    Output("choropleth2", "figure"), 
    [Input("choropleth-dataclass", "value")],
    Input('my-slider', 'value'),
    [Input('age_group', 'value')],
    [Input("sc2_conf", "value")],
    [Input("sc2_init_infector", "value")],
    [Input("sc2_vaccine_strategy", "value")]
)
def display_choropleth2(data_class, slider_val, age, sc_conf, sc_init_inf, sc_vac_strategy):
    models.data_choropleth.update_df_by_age_scenario(2, age, sc_conf, sc_init_inf, sc_vac_strategy)
    return generate_choropleth(2, data_class, slider_val)

    # # data_frame = models.data_choropleth.Data_choropleth.df if scenario_dropdown==1 else models.data_choropleth.Data_choropleth.df_another
    # # data_frame = data_frame[data_frame['Period']==slider_val ]
    # column = data_type+'_'+ data_class
    # data_frame = models.data_choropleth.dict_df_by_period[scenario_dropdown][slider_val-1]
    # fig = px.choropleth_mapbox(data_frame=data_frame , 
    #     geojson='https://raw.githubusercontent.com/JJShen2000/Covid-19-Vaccine-Simulator/main/src/visualization/twTown.geo.json', 
    #     locations='Town', 
    #     featureidkey='properties.name',  
    #     color=column,
    #     opacity=0.7,
    #     color_continuous_scale=px.colors.sequential.Reds,
    #     range_color=(models.data_choropleth.range_color_classes[column]),
    #     # animation_frame='Period',
    #     height=600,
    #     width=700,
    #     mapbox_style="open-street-map",
    #     center={"lat": 24, "lon": 120},
    #     zoom=6
    #     )
    # fig.update_geos(showcountries=False, showcoastlines=False, showland=False, fitbounds="locations")
    # fig.update_layout(margin={"r":0,"t":0,"l":0,"b":0})

    # return fig


# Update the classes selection dropdown of choropleth
@app.callback(
    dash.dependencies.Output('choropleth-dataclass', 'options'),
    [dash.dependencies.Input('choropleth-datatype', 'value')]
)
def update_choro_class_dropdown(type):
    if type == 'num':
        return [{'label': i, 'value': i} for i in models.data_choropleth.classes_num]
    elif type == 'ratio':
        return [{'label': i, 'value': i} for i in models.data_choropleth.classes_ratio]

@app.callback(
    dash.dependencies.Output('choropleth-dataclass', 'value'),
    [dash.dependencies.Input('choropleth-datatype', 'value')]
)
def update_dropdown(type):
    if type == 'num':
        return models.data_choropleth.classes_num[0]
    elif type == 'ratio':
        return models.data_choropleth.classes_ratio[0]


@app.callback(
    Output("line-chart", "figure"), 
    [Input("line-chart-datatype", "value")], 
    [Input("line-chart-dataclass", "value")],
    [Input('line-chart-country', 'value')],
    [Input('line-chart-town', 'value')],
    [dash.dependencies.Input('age_group_lc1', 'value')]
)
def update_line_chart(data_type, data_class, country, town, age_group):
    models.data_line_chart.update_df(country, town, age_group)

    fig = px.line(models.data_line_chart.df, 
        x="Period", y=data_class, color='scenario',height=500)

    fig.update_layout(
        legend_title="Scenario",
        font=dict(
            size=18
        )
    )

    fig.update_traces(mode="markers+lines")

    fig.update_xaxes(showspikes=True, spikecolor="green", spikesnap="cursor", spikemode="across")
    fig.update_yaxes(showspikes=True, spikecolor="orange", spikethickness=2)
    fig.update_layout(spikedistance=1000, hoverdistance=100)
    return fig


@app.callback(
    Output("line-chart2", "figure"), 
    [Input("line-chart2-datatype", "value")], 
    [Input('line-chart2-country', 'value')],
    [Input('line-chart2-town', 'value')],
    [Input('age_group_lc12', 'value')],
    [Input("lc-state-scenario_conf", "value")],
    [Input("lc-state-scenario_init_infector", "value")],
    [Input("lc-state-scenario_vaccine_strategy", "value")]
)
def update_line_chart_by_state(data_type, country, town, age_group, sc_conf, sc_init_inf, sc_vac_strategy):
    models.data_line_chart_state.update_df_by_age_scenario(age_group, sc_conf, sc_init_inf, sc_vac_strategy)
    mask = ((models.data_line_chart_state.df['Town']==town) & (models.data_line_chart_state.df['Country']==country))
    
    local_df = models.data_line_chart_state.df[mask]
    # fig = px.line(models.data_line_chart_state.df[mask], 
    #     x="Period", y=data_class, color='scenario',height=500)
    fig = go.Figure()

    if data_type == 'ratio':
        state_list = models.data_line_chart_state.classes_ratio 
    else:
        state_list = models.data_line_chart_state.classes_num

    for col in state_list:
        if col == 'num_infected':
            col_name = 'num_infectious'
        elif col == 'num_vaccined':
            col_name = 'num_vaccinated'
        else:
            col_name = col
        fig.add_trace(go.Scatter(x=local_df["Period"], y=local_df[col],
                            name=col_name
                            ))
    # fig.add_trace(go.Scatter(x=local_df["Period"], y=local_df['Deaths (ratio)'], name = 'Deaths (ratio)',
    #                      line=dict(color='royalblue', width=4)))
    # fig.add_trace(go.Scatter(x=local_df["Period"], y=local_df[' (ratio)'], name = 'Deaths (ratio)',
    #                      line=dict(color='royalblue', width=4)))

    fig.update_layout(
        legend_title="State",
        font=dict(
            size=18
        )
    )

    fig.update_traces(mode="markers+lines")

    fig.update_xaxes(showspikes=True, spikecolor="green", spikesnap="cursor", spikemode="across")
    fig.update_yaxes(showspikes=True, spikecolor="orange", spikethickness=2)
    fig.update_layout(spikedistance=1000, hoverdistance=100)
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


# @app.callback(
#     [dash.dependencies.Output('age_group', 'value')],
#     [dash.dependencies.Input('age_group', 'value')],
# )
# def update_age_group_choro(age_id):
#     models.data_choropleth.update_df(age_id)
#     return dash.no_update 

# @app.callback(
#     [dash.dependencies.Output('age_group_lc1', 'value')],
#     [dash.dependencies.Input('age_group_lc1', 'value')],
# )
# def update_age_group_lc1(age_id):
#     models.data_line_chart.update_age(age_id)
#     return dash.no_update 


# Update the classes selection dropdown of choropleth
@app.callback(
    dash.dependencies.Output('line-chart-dataclass', 'options'),
    [dash.dependencies.Input('line-chart-datatype', 'value')]
)
def update_choro_class_dropdown(type):
    if type == 'num':
        return [{'label': i, 'value': i} for i in models.data_line_chart.classes_num]
    elif type == 'ratio':
        return [{'label': i, 'value': i} for i in models.data_line_chart.classes_ratio]

@app.callback(
    dash.dependencies.Output('line-chart-dataclass', 'value'),
    [dash.dependencies.Input('line-chart-datatype', 'value')]
)
def update_dropdown(type):
    if type == 'num':
        return models.data_line_chart.classes_num[0]
    elif type == 'ratio':
        return models.data_line_chart.classes_ratio[0]


# Update the classes selection dropdown of lc2
@app.callback(
    dash.dependencies.Output('line-chart2-dataclass', 'options'),
    [dash.dependencies.Input('line-chart2-datatype', 'value')]
)
def update_choro_class_dropdown(type):
    if type == 'num':
        return [{'label': i, 'value': i} for i in models.data_line_chart_state.classes_num]
    elif type == 'ratio':
        return [{'label': i, 'value': i} for i in models.data_line_chart_state.classes_ratio]

@app.callback(
    dash.dependencies.Output('line-chart2-dataclass', 'value'),
    [dash.dependencies.Input('line-chart2-datatype', 'value')]
)
def update_dropdown(type):
    if type == 'num':
        return models.data_line_chart_state.classes_num[0]
    elif type == 'ratio':
        return models.data_line_chart_state.classes_ratio[0]


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


# Update dropdown of line chart2
@app.callback(
    dash.dependencies.Output('line-chart2-town', 'options'),
    [dash.dependencies.Input('line-chart2-country', 'value')]
)
def update_dropdown(name):
    return [{'label': i, 'value': i} for i in models.data_line_chart.tract_option_dict[name]]

@app.callback(
    dash.dependencies.Output('line-chart2-town', 'value'),
    [dash.dependencies.Input('line-chart2-country', 'value')]
)
def update_dropdown(name):
    return models.data_line_chart.tract_option_dict[name][0]


@app.callback(
    Output('page-content', 'children'),
    Input('url', 'pathname')
)
def render_page_content(pathname):
    if pathname == '/' or pathname == '':
        return html.Div(className="content", children=[html.H1('This is the visualization of Covid-19 simulator!'), html.A('Github repo of model.', href='https://github.com/JJShen2000/Covid-19-Vaccine-Simulator')])
    elif pathname == '/choropleth':
        return views.choropleth.choropleth_page
    elif pathname == '/line_chart':
        return views.line_chart.line_chart_page
    elif pathname == '/line_chart_state':
        return views.line_chart_state_page.line_chart_state_page

    return html.Div(className="content", children=html.H1('404！'))


if __name__ == '__main__':
    app.run_server("192.168.107.31")
