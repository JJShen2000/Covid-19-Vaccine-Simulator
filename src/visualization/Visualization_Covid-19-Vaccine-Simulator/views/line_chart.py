from dash import html
from dash import dcc
import dash_gif_component as Gif

import models.data_line_chart as data_l


line_chart_page = html.Div(
    [
        html.Div(
            className="content",
            children=[
                html.H2('Line Chart'),
                html.H3('Select age group'),
                    dcc.Dropdown(
                        id="age_group_lc1",
                        options=[
                            {'label': data_l.age_group_label[i], 'value': i} for i in data_l.age_group_id
                        ],
                        value=data_l.age_group_id[-1],
                        style={"width": "40%"}
                ),
                html.H3("Choose the data you want to know:"),
                html.Div(
                    [
                        html.Div(
                            [
                                dcc.Dropdown(
                                    id="line-chart-datatype",
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
                                    id="line-chart-dataclass"
                                )
                            ],
                            style={"width": "20%", "display": "inline-block", "margin-right": "5%"}
                        )
                    ],
                    style={"display": "inline-block","width": "100%"}
                ),
                # dcc.RadioItems(
                #     id="line-chart-data-class",
                #     options=[{"label": x, "value": x} 
                #             for x in data_l.classes],
                #     value=data_l.classes[0],
                #     labelStyle={'display': 'inline-block'}
                # ),
                html.H3("Choose the tract:"),
                html.Div(
                    [dcc.Dropdown(
                            id="line-chart-country",
                            options=[
                                {'label': i, 'value': i} for i in data_l.tracts
                            ],
                            value='全台'
                    )],
                    style={"width": "10%", "display": "inline-block", "margin-right": "5%"}
                ),
                html.Div(
                    [dcc.Dropdown(
                            id="line-chart-town"
                    )],
                    style={"width": "10%", "display": "inline-block"}
                ),
                dcc.Graph(id="line-chart"),
                # html.Div(
                #     [Gif.GifPlayer(
                #         gif='assets/test.gif',still='assets/test.png', autoplay=True
                #     )]
                # )
            ]
        )
    ]
)
