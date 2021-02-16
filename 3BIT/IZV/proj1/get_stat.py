"""
Project: IZV projekt part 1
Author: Michal Koval <xkoval17@stud.fit.vutbr.cz>
"""

from download import DataDownloader
import matplotlib.pyplot as plt
import numpy as np
import os
import sys
import argparse


"""
IMPORTANT NOTE:
Graph may look messed up.
Graph was made to look properly good as saved file and to fit to A4 format of paper.
Reasons are that during development i had connected 2 monitors to my notebook, so 
i had 3 screens with different resolutions and on every screen the showed graph looked differently, so i didn't 
know for which resolution should i optimize the graph. 
"""

def plot_stat(data_source, fig_location=None, show_figure=False):
    """
    Processes input data and plots the bar graph displaying number of accidents
    :param data_source: Data used to plot the graph
    :param fig_location: Folder where graph will be saved
    :param show_figure: If set to True shows graph
    :return:
    """
    # Check is data was given
    if not data_source:
        print("Missing data to plot", file=sys.stderr)

    # Getting needed information from data
    data = process_data(data_source)
    num_years = len(data)
    data = dict(sorted(data.items()))
    # Creating the graphs
    fig = plt.figure(figsize=(8.27, 11.69))
    plt.subplots_adjust(hspace=0.8)
    sub_plot_id = 1

    # Find yliem
    max_y = find_ylim(data)

    for y in data:
        plt.subplot(num_years, 1, sub_plot_id)
        X = [2 * y for y in np.arange(len(data[y]))]
        plt.ylim(top=max_y)
        plt.yticks([y for y in range(0, max_y, max_y // 5)])
        plt.xlabel("Kraje")
        plt.ylabel("Počet nehôd")
        plt.title(y)
        plt.xticks(X, data[y].keys())
        vals = [y[0] for y in data[y].values()]
        plt.bar(X, vals, color=(0.0, 0.63, 0.92, 0.4))
        bar_plot = plt.bar(X, vals, width=1.4)
        for idx, rect in enumerate(bar_plot):
            plt.text(rect.get_x() + rect.get_width() / 2., 0, vals[idx], fontdict={"size": "7"}, ha='center',
                     va='bottom', rotation=0)

        ranks = [y[1] for y in data[y].values()]
        for idx, rect in enumerate(bar_plot):
            height = rect.get_height()
            plt.text(rect.get_x() + rect.get_width() / 2., height, ranks[idx], fontdict={"size": "7"}, ha='center',
                     va='bottom', rotation=0)
        sub_plot_id += 1

    # Saving and/or showing figure
    if fig_location:
        folder = fig_location[0:fig_location.rfind('/')]
        check_folder(folder)
        plt.savefig(fig_location)
    if show_figure:
        plt.show()


def find_ylim(data):
    """
    Calculates y top limit for given data
    :param data: Input data to calcute from
    :return: top y_lim
    """
    max_y = 0
    for y in data:
        vals = [y[0] for y in data[y].values()]
        m_vals = max(vals)
        if m_vals > max_y:
            max_y = round(m_vals, -3) + 2000

    return max_y


def check_folder(folder):
    """
    Checks whether folders exists, if not creates it
    :param folder: Path to folder
    """

    if os.path.exists(folder):
        if os.path.isdir(folder):
            return
    os.makedirs(folder)

def process_data(data_source):
    """
    Extracts needed information from plotting of graph from input data
    :param data_source: Input data
    :return: Processed data
    """
    # Getting the years and counties
    counties = set(data_source[1][0])
    years = []
    for date in data_source[1][4]:
        if date.item().year not in years:
            years.append(date.item().year)
    # Initializing structure
    ret_dict = dict()
    for y in years:
        ret_dict[y] = dict()
        for c in counties:
            ret_dict[y][c] = 0
    # Counting the accident
    for i in range(len(data_source[1][0])):
        county = data_source[1][0][i]
        year = data_source[1][4][i].item().year
        ret_dict[year][county] += 1
    # Ranking counties for each year
    for year in ret_dict:
        sorted_list = [k for k, v in sorted(ret_dict[year].items(), key=lambda item: item[1])]
        sorted_list.reverse()
        for c in ret_dict[year]:
            ret_dict[year][c] = (ret_dict[year][c], sorted_list.index(c) + 1)

    return ret_dict


if __name__ == "__main__":
    # Parsing arguments
    parser = argparse.ArgumentParser()
    parser.add_argument("--show_figure", action="store_true", help="Shows graph if set")
    parser.add_argument("--fig_location", help="Folder where graph will be stored")
    args = parser.parse_args()

    show = False
    loc = None
    if args.show_figure:
        show = True
    if args.fig_location:
        loc = args.fig_location

    plot_stat(DataDownloader().get_list(), loc, show)
