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
                    html.Div(
                        [
                            html.Div(
                                [
                                    dcc.Dropdown(
                                        id="choropleth-datatype",
                                        options=[
                                            {'label': 'Number' if i == 'num' else 'Ratio', 'value': i} for i in ['num', 'ratio']
                                        ],
                                        value='num',
                                        # style={"width": "50%", "display": "inline-block", "margin-right": "5%"}
                                    )
                                ],
                                style={"width": "10%", "display": "inline-block", "margin-right": "10px"}
                            ),
                            html.P('of', style={"display": "inline-block", "margin-right": "10px", 'font-size':25}),
                            html.Div(
                                [
                                    dcc.Dropdown(
                                        id="choropleth-dataclass",
                                    )
                                ],
                                style={"width": "20%", "display": "inline-block", "margin-right": "5%"}
                            )
                        ],
                        style={"display": "inline-block","width": "100%"}
                    ),
                    # ),
                    # dcc.RadioItems(
                    #     id='candidate', 
                    #     options=[{'value': x, 'label': data_c.classes2lable[x]} for x in data_c.classes],
                    #     value=data_c.classes[0],
                    #     labelStyle={'display': 'inline-block'}
                    # )
                    html.H3('Select age group'),
                    dcc.Dropdown(
                        id="age_group",
                        options=[
                            {'label': data_c.age_group_label[i], 'value': i} for i in data_c.age_group_id
                        ],
                        value=data_c.age_group_id[-1],
                        style={"width": "40%"}
                    ),
                ]
            ),
            html.Div(
                className="content_left",
                children=[
                    html.H3('Select Scenario 1.'),
                    dcc.Dropdown(
                        id="sc1_conf",
                        options=[
                            {'label': data_c.scenario_conf[i], 'value': i} for i in data_c.scenario_conf_keys
                        ],
                        value=data_c.scenario_conf_keys[0]
                    ),
                    dcc.Dropdown(
                        id="sc1_init_infector",
                        options=[
                            {'label': data_c.scenario_init_infectors[i], 'value': i} for i in data_c.scenario_init_infectors_keys
                        ],
                        value=data_c.scenario_init_infectors_keys[0]
                    ),
                    dcc.Dropdown(
                        id="sc1_vaccine_strategy",
                        options=[
                            {'label': data_c.scenario_vaccine_strategies[i], 'value': i} for i in data_c.scenario_vaccine_strategies_keys
                        ],
                        value=data_c.scenario_vaccine_strategies_keys[0]
                    ),
                    dcc.Graph(id="choropleth1"),
                ]
            ),
            html.Div(
                className="content_right",
                children=[
                    html.H3('Select Scenario 2.'),
                    dcc.Dropdown(
                        id="sc2_conf",
                        options=[
                            {'label': data_c.scenario_conf[i], 'value': i} for i in data_c.scenario_conf_keys
                        ],
                        value=data_c.scenario_conf_keys[1]
                    ),
                    dcc.Dropdown(
                        id="sc2_init_infector",
                        options=[
                            {'label': data_c.scenario_init_infectors[i], 'value': i} for i in data_c.scenario_init_infectors_keys
                        ],
                        value=data_c.scenario_init_infectors_keys[1]
                    ),
                    dcc.Dropdown(
                        id="sc2_vaccine_strategy",
                        options=[
                            {'label': data_c.scenario_vaccine_strategies[i], 'value': i} for i in data_c.scenario_vaccine_strategies_keys
                        ],
                        value=data_c.scenario_vaccine_strategies_keys[1]
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