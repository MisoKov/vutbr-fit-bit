#!/usr/bin/env python3.8
# coding=utf-8

from matplotlib import pyplot as plt
import matplotlib.dates as md
import pandas as pd
import seaborn as sns
import numpy as np
import os
import sys


import warnings
warnings.filterwarnings("ignore")

bytes_in_mb = 1048576

# Ukol 1: nacteni dat
def get_dataframe(filename: str = "accidents.pkl.gz", verbose: bool = False) -> pd.DataFrame:
    # Check file existence
    if not os.path.exists(filename):
        print("File: '"+filename+"' not found", file=sys.stderr)
        quit(1)
    try:
        df_old = pd.read_pickle(filename, compression="gzip")
    except Exception:
        print("Invalid file: '" + filename + "'", file=sys.stderr)
        quit(1)

    # Format data
    df = df_old.astype("category")
    df["region"] = df_old["region"]
    df["date"] = df.p2a.astype("datetime64")
    df["p13a"] = df.p13a.astype("int16")
    df["p53"] = df.p53.astype("int16")
    df["p12"] = df.p12.astype("int16")
    df["p13b"] = df.p13b.astype("int16")
    df["p13c"] = df.p13c.astype("int16")
    df.drop("p2a", axis=1, inplace=True)

    # Print sizes
    if verbose:
        print("orig_size={:.1f} MB".format(df_old.memory_usage(deep=True).sum()/bytes_in_mb))
        print("new_size={:.1f} MB".format(df.memory_usage(deep=True).sum()/bytes_in_mb))

    return df

# Ukol 2: následky nehod v jednotlivých regionech
def plot_conseq(df: pd.DataFrame, fig_location: str = None,
                show_figure: bool = False):
    # Preparing data
    df_sliced = df[["p13a", "p13b", "p13c", "region"]]
    groups = df_sliced.groupby("region").agg(["sum"])
    groups_accidents = df_sliced.groupby("region")["region"].count()
    groups["count"] = groups_accidents
    groups.sort_values('count', ascending=False, inplace=True)
    groups = groups.astype('int')

    x_labels = groups.index.values
    deaths = [y[0] for y in groups["p13a"].to_numpy()]
    severe_injury = [y[0] for y in groups["p13b"].to_numpy()]
    light_light = [y[0] for y in groups["p13c"].to_numpy()]
    accidents = [y for y in groups["count"].to_numpy()]


    # Creating the graphs
    X = [2 * y for y in np.arange(len(x_labels))]
    fig = plt.figure(figsize=(8.27, 11.69))
    fig.suptitle('Summary of car accidents in CR', fontsize=16)
    fig.tight_layout()

    fig.subplots_adjust(top=0.93)
    #plt.subplots_adjust(hspace=0.4)

    # Deaths
    ax = plt.subplot(4, 1, 1)
    ax.set_facecolor("#E0E0E0")
    plt.ylabel("Number of deaths")
    plt.title("Deaths")
    plt.tick_params(
        axis='x',
        which='both',
        bottom=False,
        top=False,
        labelbottom=False)
    plt.bar(x=X, height=deaths, width=1.4, color="#c0140a", zorder=3)
    ax.yaxis.grid(True, zorder=0)
    plt.xticks(X, x_labels)

    # Severe injuries
    ax = plt.subplot(4, 1, 2)
    ax.set_facecolor("#E0E0E0")
    plt.title("Severe injuries")
    plt.ylabel("Number of severe injuries")
    plt.tick_params(
        axis='x',
        which='both',
        bottom=False,
        top=False,
        labelbottom=False)
    plt.bar(x=X, height=severe_injury, width=1.4, color="#fb7c24", zorder=3)
    ax.yaxis.grid(True, zorder=0)
    plt.xticks(X, x_labels)

    # Light injuries
    ax = plt.subplot(4, 1, 3)
    ax.set_facecolor("#E0E0E0")
    plt.title("Light injuries")
    plt.ylabel("Number of light injuries")
    plt.tick_params(
        axis='x',
        which='both',
        bottom=False,
        top=False,
        labelbottom=False)
    plt.bar(x=X, height=light_light, width=1.4, color="#e6ab00", zorder=3)
    ax.yaxis.grid(True, zorder=0)
    plt.xticks(X, x_labels)

    # Overall accidents
    ax = plt.subplot(4, 1, 4)
    ax.set_facecolor("#E0E0E0")
    plt.title("Overall accidents")
    plt.ylabel("Number of accidents")
    plt.tick_params(
        axis='x',
        which='both',
        bottom=False,
        top=False,
        labelbottom=True)
    plt.bar(x=X, height=accidents, width=1.4, zorder=3)
    ax.yaxis.grid(True, zorder=0)
    plt.xticks(X, x_labels)


    plt.xlabel("Counties")

    # Show/Save
    if fig_location:
        plt.savefig(fig_location)
    if show_figure:
        plt.show()


# Ukol3: příčina nehody a škoda
def plot_damage(df: pd.DataFrame, fig_location: str = None,
                show_figure: bool = False):
    # Preparing data
    df_sliced = df[["p53", "p12", "region"]]
    df_sliced["p53"] = df_sliced["p53"].div(10).astype("int")
    df_sliced["bins_skody"] = pd.cut(x=df_sliced["p53"], bins=[0, 50, 200, 500, 1000, float("inf")], labels=["< 50", "50 - 200", "200 - 500", "500 - 1 000", "> 1 000"], include_lowest=True)
    df_sliced["bins_priciny"] = pd.cut(x=df_sliced["p12"], bins=[0, 200, 300, 400, 500, 600, 615], labels=["nezaviněná řidičem", "nepřiměřená rychlost jízdy", "nesprávné předjíždění", "nedání přednosti v jízdě", "nesprávný způsob jízdy", "technická závada vozidla"])
    df_sliced = df_sliced.drop(columns=["p12"])

    # Dividing data to regions
    pha = df_sliced.loc[df["region"] == "PHA"]
    pha = pha.drop(columns=["region"])
    pha = pha.groupby(["bins_skody", "bins_priciny"]).agg(["count"])
    pha_vals = [y for y in pha["p53"]["count"].values]

    kvk = df_sliced.loc[df["region"] == "KVK"]
    kvk = kvk.drop(columns=["region"])
    kvk = kvk.groupby(["bins_skody", "bins_priciny"]).agg(["count"])
    kvk_vals = [y for y in kvk["p53"]["count"].values]

    jhm = df_sliced.loc[df["region"] == "JHM"]
    jhm = jhm.drop(columns=["region"])
    jhm = jhm.groupby(["bins_skody", "bins_priciny"]).agg(["count"])
    jhm_vals = [y for y in jhm["p53"]["count"].values]

    vys = df_sliced.loc[df["region"] == "VYS"]
    vys = vys.drop(columns=["region"])
    vys = vys.groupby(["bins_skody", "bins_priciny"]).agg(["count"])
    vys_vals = [y for y in vys["p53"]["count"].values]

    # Creating the graphs
    N = 5
    X = np.arange(N)
    width = 0.13
    x_ticks = ["< 50", "50 - 200", "200 - 500", "500 - 1 000", "> 1 000"]
    fig = plt.figure(figsize=(14, 11))
    fig.suptitle('Zhrnutie hmotných škôd a ich príčin', fontsize=16)
    fig.subplots_adjust(top=0.93)
    fig.tight_layout()

    # PHA
    ax = fig.add_subplot(221)
    ax.set_facecolor("#E0E0E0")
    plt.ylabel("Počet")
    ax.set_ylim([1e0, 1e5])
    ax.set_yscale("symlog")
    ax.set_xticks(X + width*2.3)
    ax.set_xticklabels(x_ticks)
    plt.tick_params(
        axis='x',
        which='both',
        bottom=False,
        top=False,
        labelbottom=True)
    ax.yaxis.grid(True, zorder=0)
    plt.xlabel("Škoda [tisíc KČ]")
    plt.title("VYS")
    # Dividing region data to bars values according to categories
    p1 = [pha_vals[i] for i in range(0, len(pha_vals), 6)]
    p2 = [pha_vals[i] for i in range(1, len(pha_vals), 6)]
    p3 = [pha_vals[i] for i in range(2, len(pha_vals), 6)]
    p4 = [pha_vals[i] for i in range(3, len(pha_vals), 6)]
    p5 = [pha_vals[i] for i in range(4, len(pha_vals), 6)]
    p6 = [pha_vals[i] for i in range(5, len(pha_vals), 6)]
    # Drawing bars
    ax.bar(x=X - 0.1, height=p1, width=width, zorder=3)
    ax.bar(x=X + width - 0.08, height=p2, width=width, zorder=3)
    ax.bar(x=X + width * 2 - 0.06, height=p3, width=width, zorder=3)
    ax.bar(x=X + width * 3 - 0.04, height=p4, width=width, zorder=3)
    ax.bar(x=X + width * 4 - 0.02, height=p5, width=width, zorder=3)
    ax.bar(x=X + width * 5, height=p6, width=width, zorder=3)

    # KVK
    ax = fig.add_subplot(222)
    ax.set_facecolor("#E0E0E0")
    plt.ylabel("Počet")
    ax.set_ylim([1e0, 1e5])
    ax.set_yscale("symlog")
    ax.set_xticks(X + width * 2.3)
    ax.set_xticklabels(x_ticks)
    plt.tick_params(
        axis='x',
        which='both',
        bottom=False,
        top=False,
        labelbottom=True)
    ax.yaxis.grid(True, zorder=0)
    plt.xlabel("Škoda [tisíc KČ]")
    plt.title("VYS")
    # Dividing region data to bars values according to categories
    p1 = [kvk_vals[i] for i in range(0, len(kvk_vals), 6)]
    p2 = [kvk_vals[i] for i in range(1, len(kvk_vals), 6)]
    p3 = [kvk_vals[i] for i in range(2, len(kvk_vals), 6)]
    p4 = [kvk_vals[i] for i in range(3, len(kvk_vals), 6)]
    p5 = [kvk_vals[i] for i in range(4, len(kvk_vals), 6)]
    p6 = [kvk_vals[i] for i in range(5, len(kvk_vals), 6)]
    # Drawing bars
    ax.bar(x=X - 0.1, height=p1, width=width, zorder=3)
    ax.bar(x=X + width - 0.08, height=p2, width=width, zorder=3)
    ax.bar(x=X + width * 2 - 0.06, height=p3, width=width, zorder=3)
    ax.bar(x=X + width * 3 - 0.04, height=p4, width=width, zorder=3)
    ax.bar(x=X + width * 4 - 0.02, height=p5, width=width, zorder=3)
    ax.bar(x=X + width * 5, height=p6, width=width, zorder=3)

    # JHM
    ax = fig.add_subplot(223)
    ax.set_facecolor("#E0E0E0")
    plt.ylabel("Počet")
    ax.set_ylim([1e0, 1e5])
    ax.set_yscale("symlog")
    ax.set_xticks(X + width * 2.3)
    ax.set_xticklabels(x_ticks)
    plt.tick_params(
        axis='x',
        which='both',
        bottom=False,
        top=False,
        labelbottom=True)
    plt.xlabel("Škoda [tisíc KČ]")
    ax.yaxis.grid(True, zorder=0)
    plt.title("VYS")
    # Dividing region data to bars values according to categories
    p1 = [jhm_vals[i] for i in range(0, len(jhm_vals), 6)]
    p2 = [jhm_vals[i] for i in range(1, len(jhm_vals), 6)]
    p3 = [jhm_vals[i] for i in range(2, len(jhm_vals), 6)]
    p4 = [jhm_vals[i] for i in range(3, len(jhm_vals), 6)]
    p5 = [jhm_vals[i] for i in range(4, len(jhm_vals), 6)]
    p6 = [jhm_vals[i] for i in range(5, len(jhm_vals), 6)]
    # Drawing bars
    ax.bar(x=X - 0.1, height=p1, width=width, zorder=3)
    ax.bar(x=X + width - 0.08, height=p2, width=width, zorder=3)
    ax.bar(x=X + width * 2 - 0.06, height=p3, width=width, zorder=3)
    ax.bar(x=X + width * 3 - 0.04, height=p4, width=width, zorder=3)
    ax.bar(x=X + width * 4 - 0.02, height=p5, width=width, zorder=3)
    ax.bar(x=X + width * 5, height=p6, width=width, zorder=3)

    # VYS
    ax = fig.add_subplot(224)
    ax.set_facecolor("#E0E0E0")
    plt.ylabel("Počet")
    ax.set_ylim([1e0, 1e5])
    ax.set_yscale("symlog")
    ax.set_xticks(X + width * 2.3)
    ax.set_xticklabels(x_ticks)
    plt.tick_params(
        axis='x',
        which='both',
        bottom=False,
        top=False,
        labelbottom=True)
    plt.xlabel("Škoda [tisíc KČ]")
    ax.yaxis.grid(True, zorder=0)
    plt.title("VYS")
    # Dividing region data to bars values according to categories
    p1 = [vys_vals[i] for i in range(0, len(vys_vals), 6)]
    p2 = [vys_vals[i] for i in range(1, len(vys_vals), 6)]
    p3 = [vys_vals[i] for i in range(2, len(vys_vals), 6)]
    p4 = [vys_vals[i] for i in range(3, len(vys_vals), 6)]
    p5 = [vys_vals[i] for i in range(4, len(vys_vals), 6)]
    p6 = [vys_vals[i] for i in range(5, len(vys_vals), 6)]
    # Drawing bars
    b1 = ax.bar(x=X - 0.1, height=p1, width=width, zorder=3)
    b2 = ax.bar(x=X + width - 0.08, height=p2, width=width, zorder=3)
    b3 = ax.bar(x=X + width * 2 - 0.06, height=p3, width=width, zorder=3)
    b4 = ax.bar(x=X + width * 3 - 0.04, height=p4, width=width, zorder=3)
    b5 = ax.bar(x=X + width * 4 - 0.02, height=p5, width=width, zorder=3)
    b6 = ax.bar(x=X + width * 5, height=p6, width=width, zorder=3)

    # Adding legend
    fig.subplots_adjust(right=0.83)
    ax.legend(handles=(b1, b2, b3, b4, b5, b6), frameon=False, bbox_to_anchor=(1.55, 1.1), title="Príčina nehody", loc='center right',
    labels=["nezaviněná řidičem", "nepřiměřená rychlost jízdy", "nesprávné předjíždění", "nedání přednosti v jízdě", "nesprávný způsob jízdy", "technická závada vozidla"])

    # Show/save
    if fig_location:
        plt.savefig(fig_location)
    if show_figure:
        plt.show()

# Ukol 4: povrch vozovky
def plot_surface(df: pd.DataFrame, fig_location: str = None,
                 show_figure: bool = False):
    # Preparing data
    df_sliced = df[["p16", "date", "region"]]
    cross = pd.crosstab(index=[df_sliced["region"], df_sliced["date"]], columns=df_sliced["p16"])

    # Dividing region data
    plk = cross.loc["PLK",:]
    plk = plk.groupby(pd.Grouper(freq="M")).sum()
    plk_index = plk.index
    plk_vals = []
    for c in plk.columns:
        plk_vals.append([y for y in plk[c]])


    kvk = cross.loc["KVK",:]
    kvk = kvk.groupby(pd.Grouper(freq="M")).sum()
    kvk_index = kvk.index
    kvk_vals = []
    for c in kvk.columns:
        kvk_vals.append([y for y in kvk[c]])

    stc = cross.loc["STC",:]
    stc = stc.groupby(pd.Grouper(freq="M")).sum()
    stc_index = stc.index
    stc_vals = []
    for c in stc.columns:
        stc_vals.append([y for y in stc[c]])

    olk = cross.loc["OLK",:]
    olk = olk.groupby(pd.Grouper(freq="M")).sum()
    olk_index = olk.index
    olk_vals = []
    for c in olk.columns:
        olk_vals.append([y for y in olk[c]])

    # Creating figure
    fig = plt.figure(figsize=(17, 9))
    fig.suptitle('Stav vozovky v jednotlivých měsících', fontsize=16)
    fig.subplots_adjust(top=0.90)
    fig.tight_layout()

    # STC
    ax = fig.add_subplot(221)
    ax.set_facecolor("#E0E0E0")
    ax.xaxis.set_major_locator(md.YearLocator())
    ax.xaxis.set_major_formatter(md.DateFormatter('%Y'))
    for p in stc_vals:
        plt.plot(stc_index, p, linewidth=0.8, zorder=3)
    plt.title('STC')
    ax.grid(True, zorder=0)
    plt.tick_params(
        axis='x',
        which='both',
        bottom=False,
        top=False,
        labelbottom=False)
    plt.ylabel('Počet nehod')

    # PLK
    ax = fig.add_subplot(222)
    ax.set_facecolor("#E0E0E0")
    ax.xaxis.set_major_locator(md.YearLocator())
    ax.xaxis.set_major_formatter(md.DateFormatter('%Y'))

    for p in plk_vals:
        plt.plot(plk_index, p, linewidth=0.8, zorder=3)
    plt.title('PLK')
    ax.grid(True, zorder=0)
    plt.tick_params(
        axis='x',
        which='both',
        bottom=False,
        top=False,
        labelbottom=False)

    # KVK
    ax = fig.add_subplot(223)
    ax.set_facecolor("#E0E0E0")
    ax.xaxis.set_major_locator(md.YearLocator())
    ax.xaxis.set_major_formatter(md.DateFormatter('%Y'))
    for p in kvk_vals:
        plt.plot(kvk_index, p, linewidth=0.8, zorder=3)
    plt.title('KVK')
    ax.grid(True, zorder=0)
    plt.tick_params(
        axis='x',
        which='both',
        bottom=False,
        top=False,
        labelbottom=True)
    plt.xlabel('Datum vzniku nehody')
    plt.ylabel('Počet nehod')

    # OLK
    ax = fig.add_subplot(224)
    ax.set_facecolor("#E0E0E0")
    ax.xaxis.set_major_locator(md.YearLocator())
    ax.xaxis.set_major_formatter(md.DateFormatter('%Y'))
    line_handles = []
    for p in olk_vals:
        line, = plt.plot(olk_index, p, linewidth=0.8, zorder=3)
        line_handles.append(line)
    plt.title('OLK')
    ax.grid(True, zorder=0)
    plt.tick_params(
        axis='x',
        which='both',
        bottom=False,
        top=False,
        labelbottom=True)
    plt.xlabel('Datum vzniku nehody')

    # Adding legend
    fig.subplots_adjust(right=0.84)
    ax.legend(handles=line_handles, frameon=False, bbox_to_anchor=(1.51, 1.1), title="Stav vozovky", loc='center right',
              labels=["jiný stav", "suchý neznečištěný", "suchý znečištěný", "mokrý", "bláto",
                      "náledí, ujetý sníh - posypané", "náledí, ujetý sníh - neposypané", "rozlitý olej, nafta apod.",
                      "souvislý sníh", "náhlá změna stavu"])

    # Show/save
    if fig_location:
        plt.savefig(fig_location)
    if show_figure:
        plt.show()

if __name__ == "__main__":
    pass
    # zde je ukazka pouziti, tuto cast muzete modifikovat podle libosti
    # skript nebude pri testovani pousten primo, ale budou volany konkreni ¨
    # funkce.
    df = get_dataframe("accidents.pkl.gz", True)
    plot_conseq(df, fig_location="01_nasledky.png", show_figure=True)
    plot_damage(df, "02_priciny.png", True)
    plot_surface(df, "03_stav.png", True)
