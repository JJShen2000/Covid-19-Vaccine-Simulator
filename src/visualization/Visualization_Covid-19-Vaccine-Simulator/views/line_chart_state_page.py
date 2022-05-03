from dash import html
from dash import dcc

import models.data_line_chart_state as data_l


line_chart_state_page = html.Div(
    [
        html.Div(
            className="content",
            children=[
                html.H2('Line Chart'),
                html.H3('Select a scenario'),
                dcc.Dropdown(
                    id="lc-state-scenario_conf",
                    options=[
                        {'label': data_l.scenario_conf[i], 'value': i} for i in data_l.scenario_conf_keys
                    ],
                    value=data_l.scenario_conf_keys[0]
                ),
                dcc.Dropdown(
                    id="lc-state-scenario_init_infector",
                    options=[
                        {'label': data_l.scenario_init_infectors[i], 'value': i} for i in data_l.scenario_init_infectors_keys
                    ],
                    value=data_l.scenario_init_infectors_keys[0]
                ),
                dcc.Dropdown(
                    id="lc-state-scenario_vaccine_strategy",
                    options=[
                        {'label': data_l.scenario_vaccine_strategies[i], 'value': i} for i in data_l.scenario_vaccine_strategies_keys
                    ],
                    value=data_l.scenario_vaccine_strategies_keys[0]
                ),
                # dcc.Dropdown(
                #     id="lc-state-scenario_dp",
                #     options=[
                #         {'label': i, 'value': i} for i in data_l.scenario_list
                #     ],
                #     value=data_l.scenario_list[0],
                #     style={"width": "50%"}
                # ),
                html.H3('Select age group'),
                    dcc.Dropdown(
                        id="age_group_lc12",
                        options=[
                            {'label': data_l.age_group_label[i], 'value': i} for i in data_l.age_group_id
                        ],
                        value=data_l.age_group_id[-1],
                        style={"width": "40%"}
                ),
                html.H3("Choose the data type:"),
                html.Div(
                    [
                        html.Div(
                            [
                                dcc.Dropdown(
                                    id="line-chart2-datatype",
                                    options=[
                                        {'label': 'Number' if i == 'num' else 'Ratio', 'value': i} for i in ['num', 'ratio']
                                    ],
                                    value='num',
                                    # style={"width": "50%", "display": "inline-block", "margin-right": "5%"}
                                )
                            ],
                            style={"width": "10%", "display": "inline-block", "margin-right": "10px"}
                        )
                    ],
                    style={"display": "inline-block","width": "100%"}
                ),
                html.H3("Choose the tract:"),
                html.Div(
                    [dcc.Dropdown(
                            id="line-chart2-country",
                            options=[
                                {'label': i, 'value': i} for i in data_l.tracts
                            ],
                            value='全台'
                    )],
                    style={"width": "10%", "display": "inline-block", "margin-right": "5%"}
                ),
                html.Div(
                    [dcc.Dropdown(
                            id="line-chart2-town"
                    )],
                    style={"width": "10%", "display": "inline-block"}
                ),
                dcc.Graph(id="line-chart2")
            ]
        )
    ]
)
