from dash import html
from dash import dcc

import models.data_line_chart as data


line_chart_page = html.Div(
    [
        html.Div(
            className="content",
            children=[
                html.H2('Line Chart'),
                html.H3("Choose the data you want to know:"),
                dcc.RadioItems(
                    id="line-chart-data-class",
                    options=[{"label": data.Data_line_chart.classes2lable[x], "value": x} 
                            for x in data.Data_line_chart.classes],
                    value=data.Data_line_chart.classes[0],
                    labelStyle={'display': 'inline-block'}
                ),
                html.H3("Choose the tract:"),
                html.Div(
                    [dcc.Dropdown(
                            id="line-chart-country",
                            options=[
                                {'label': i, 'value': i} for i in data.Data_line_chart.tracts
                            ],
                            value=data.Data_line_chart.tracts[0]
                    )],
                    style={"width": "15%", "display": "inline-block", "margin-right": "5%"}
                ),
                html.Div(
                    [dcc.Dropdown(
                            id="line-chart-town"
                    )],
                    style={"width": "15%", "display": "inline-block"}
                ),
                dcc.Graph(id="line-chart")
            ]
        )
    ]
)
