import plotly.graph_objects as go
import plotly.express as px
# fig = px.colors.diverging.Portland()
# fig.show()

import random

# n_colors = 25
# colors = px.colors.sample_colorscale("Edge", [n/(n_colors -1) for n in range(n_colors)])
# # fig = go.Figure()
# print(colors)

#
# print(random.randint(0, 255))
# random_color = { r: random.randint(0, 255), g: randint(0, 255), b: randint(0, 255) }
# # pastel_color = random_color.saturate( 10% ).mix( white )

# print(colors)
# for i, c in enumerate(colors):
#     fig.add_bar(x=[i], y = [15], marker_color = c, showlegend = False, name=c)
# f = fig.full_figure_for_development(warn=False)
# fig.show()

import random


def get_random_color(pastel_factor=0.5):
    return [(x + pastel_factor) / (1.0 + pastel_factor) for x in [random.uniform(0, 1.0) for i in [1, 2, 3]]]


def color_distance(c1, c2):
    return sum([abs(x[0] - x[1]) for x in zip(c1, c2)])


def generate_new_color(existing_colors, pastel_factor=0.5):
    max_distance = None
    best_color = None
    for i in range(0, 100):
        color = get_random_color(pastel_factor=pastel_factor)
        if not existing_colors:
            return color
        best_distance = min([color_distance(color, c) for c in existing_colors])
        if not max_distance or best_distance > max_distance:
            max_distance = best_distance
            best_color = color
    return best_color


# Example:

def normalize(value, min_value, max_value):
    if (max_value == min_value):
        return 0.0
    return (value - min_value) / (max_value - min_value)


if __name__ == '__main__':

    labels = ["a", "a", "b", "b", "a"]

    colors = [float(hash(s) % 256) / 256 for s in labels]
    print(colors)
    # To make your color choice reproducible, uncomment the following line:
    # random.seed(10)

    colors = []

    for i in range(0, 5):
        colors.append(generate_new_color(colors,pastel_factor = 0.5))

    colors = [[int(c*255) for c in color] for color in colors]


    colors_plotly = []

    for color in colors:
        r, g, b = color
        colors_plotly.append(f"rgb({r},{g},{b})")

    colors_qt = []
    for color in colors:
        r, g, b = color
        colors_qt.append(f"QColor({r},{g},{b})")
    # # colors = [[int(color*255)] for color in colors]
    #

    print(colors_plotly)
    print(colors_qt)


    colors_plotly = ['rgb(40, 252, 10)', 'rgb(34, 34, 115)', 'rgb(230, 216, 199)', 'rgb(142, 49, 150)', 'rgb(200, 115, 151)', 'rgb(171, 9, 57)', 'rgb(197, 142, 204)', 'rgb(192, 242, 29)', 'rgb(123, 233, 219)', 'rgb(132, 127, 222)', 'rgb(153, 232, 68)', 'rgb(42, 197, 96)', 'rgb(47, 142, 47)', 'rgb(155, 103, 4)', 'rgb(171, 19, 10)', 'rgb(199, 25, 127)', 'rgb(81, 210, 23)', 'rgb(177, 6, 178)', 'rgb(96, 196, 51)', 'rgb(187, 141, 38)', 'rgb(182, 231, 203)', 'rgb(72, 157, 111)', 'rgb(218, 118, 9)', 'rgb(26, 96, 159)', 'rgb(149, 186, 46)', 'rgb(186, 253, 17)', 'rgb(162, 124, 231)', 'rgb(254, 229, 253)', 'rgb(69, 175, 80)', 'rgb(148, 125, 234)', 'rgb(73, 226, 116)', 'rgb(108, 83, 120)', 'rgb(235, 247, 13)', 'rgb(71, 61, 166)', 'rgb(243, 167, 63)', 'rgb(138, 245, 147)', 'rgb(197, 181, 202)', 'rgb(248, 81, 183)', 'rgb(157, 148, 192)', 'rgb(56, 128, 118)', 'rgb(60, 187, 150)', 'rgb(220, 226, 91)', 'rgb(192, 162, 191)', 'rgb(7, 76, 8)', 'rgb(166, 57, 9)', 'rgb(230, 177, 75)', 'rgb(186, 63, 79)', 'rgb(245, 32, 61)', 'rgb(118, 174, 178)', 'rgb(76, 147, 61)']

    fig = go.Figure()
    for i, c in enumerate(colors_plotly):
        fig.add_bar(x=[i], y = [15], marker_color = c, showlegend = False, name=c)
    f = fig.full_figure_for_development(warn=False)
    fig.show()

