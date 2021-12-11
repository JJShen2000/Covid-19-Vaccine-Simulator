from dash import html
from dash import dcc

import models.data_choropleth as data_c


choropleth_page = html.Div(
            children=[
            html.Div(
                className="content",
                children=[
                    html.H2('Choropleth'),
                    html.H3("Choose the data you want to know:"),
                    dcc.RadioItems(
                        id='candidate', 
                        options=[{'value': x, 'label': data_c.classes2lable[x]} for x in data_c.classes],
                        value=data_c.classes[0],
                        labelStyle={'display': 'inline-block'}
                    )
                ]
            ),
            html.Div(
                className="content_left",
                children=[
                    html.H3('Select Scenario 1.'),
                    dcc.Dropdown(
                        id="scenario_dropdown1",
                        options=[
                            {'label': i, 'value': i} for i in data_c.scenarioes
                        ],
                        value=data_c.scenarioes[0]
                    ),
                    dcc.Graph(id="choropleth1"),
                ]
            ),
            html.Div(
                className="content_right",
                children=[
                    html.H3('Select Scenario 2.'),
                    dcc.Dropdown(
                        id="scenario_dropdown2",
                        options=[
                            {'label': i, 'value': i} for i in data_c.scenarioes
                        ],
                        value=data_c.scenarioes[1]
                    ),
                    dcc.Graph(id="choropleth2"),
                ]
            ),
            html.Div(
                className="line_chart",
                children=[
                    dcc.Interval(
                        id='auto-stepper',
                        interval=1*1000, # ms
                        n_intervals=0,
                        disabled=True
                    ),  
                    dcc.Slider(
                        id='my-slider',
                        min=1,
                        max=data_c.period_num,
                        step=1,
                        value=1,
                        tooltip={"placement": "bottom", "always_visible": True},
                    ),
                    html.Button("Play", id='play')
                ]
            )
        ])