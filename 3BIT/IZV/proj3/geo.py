#!/usr/bin/python3.8
# coding=utf-8
import pandas as pd
import geopandas
import matplotlib.pyplot as plt
import contextily as ctx
import sklearn.cluster
import numpy as np
# muzeze pridat vlastni knihovny



def make_geo(df: pd.DataFrame) -> geopandas.GeoDataFrame:
    """ Konvertovani dataframe do geopandas.GeoDataFrame se spravnym kodovani"""
    gdf = geopandas.GeoDataFrame(df, geometry=geopandas.points_from_xy(df["d"], df["e"]), crs="EPSG:5514")
    return gdf

def plot_geo(gdf: geopandas.GeoDataFrame, fig_location: str = None,
             show_figure: bool = False):
    """ Vykresleni grafu s dvemi podgrafy podle lokality nehody """
    # Spracovanie dat pre OLK region
    gdf = gdf[gdf["region"] == "OLK"]
    gdf = gdf.to_crs("EPSG:5514")
    minx, miny, maxx, maxy = gdf.total_bounds
    margin = 3000
    gdf = gdf[(gdf.e.notnull()) & (gdf.d.notnull())]

    # Vytvaranie figure
    fig, ax = plt.subplots(1, 2, figsize=(14, 8))
    fig.suptitle("Zobrazenie miest nehôd v OLK kraji")
    for a in ax:
        a.axis("off")

    # V obci
    ax = fig.add_subplot(121)
    ax.axis("off")
    ax.set_xlim(minx - margin, maxx + margin)
    ax.set_ylim(miny - margin, maxy + margin)
    plt.title("Nehody v OLK kraji: v obci")
    gdf[(gdf["p5a"] == 1)].plot(ax=ax, markersize=1, color="tab:red")
    ctx.add_basemap(ax, crs=gdf.crs.to_string(), source=ctx.providers.Stamen.TonerLite, alpha=0.9)

    # Mimo obce
    ax = fig.add_subplot(122)
    ax.axis("off")
    ax.set_xlim(minx - margin, maxx + margin)
    ax.set_ylim(miny - margin, maxy + margin)
    plt.title("Nehody v OLK kraji: mimo obce")
    gdf[(gdf["p5a"] == 2)].plot(ax=ax, markersize=1, color="tab:green")
    ctx.add_basemap(ax, crs=gdf.crs.to_string(), source=ctx.providers.Stamen.TonerLite, alpha=0.9)

    # Show/save
    if fig_location:
        plt.savefig(fig_location)
    if show_figure:
        plt.show()


def plot_cluster(gdf: geopandas.GeoDataFrame, fig_location: str = None,
                 show_figure: bool = False):
    """ Vykresleni grafu s lokalitou vsech nehod v kraji shlukovanych do clusteru """
    # Spracovanie dat pre OLK region
    gdf = gdf[gdf["region"] == "OLK"]
    margin = 3000
    # Odstranenie dat bez coordinatov
    gdf = gdf[(gdf.e.notnull()) & (gdf.d.notnull())]
    # Prenesenie
    gdf_c = gdf.to_crs("EPSG:5514")
    gdf_c = gdf_c.set_geometry(gdf_c.centroid).to_crs(epsg=3857)
    # Ziskanie medzi
    minx, miny, maxx, maxy = gdf_c.total_bounds
    # Vytvorenie clusterov
    coords = np.dstack([gdf_c.geometry.x, gdf_c.geometry.y]).reshape(-1, 2)
    clusters = sklearn.cluster.KMeans(n_clusters=12, random_state=2).fit(coords)
    # Zlucenie podla clusterov
    gdf_cl = gdf_c.copy()
    gdf_cl["cluster"] = clusters.labels_
    gdf_cl = gdf_cl.dissolve(by="cluster", aggfunc={"cluster": "count"}).rename(columns=dict(cluster="cnt"))
    # Centra clusterov
    clusters_coords = geopandas.GeoDataFrame(geometry=geopandas.points_from_xy(clusters.cluster_centers_[:, 0], clusters.cluster_centers_[:, 1]))
    gdf_final = gdf_cl.merge(clusters_coords, left_on="cluster", right_index=True).set_geometry("geometry_y")
    # Vykreslenie nehod
    ax = gdf_c.plot(figsize=(18, 18), markersize=1, color="tab:grey")
    ax.set_xlim(minx - margin, maxx + margin)
    ax.set_ylim(miny - margin, maxy + margin)
    plt.title("Nehody v OLK kraji")
    ax.axis("off")
    # Vykreslenie clusterov
    gdf_final.plot(ax=ax, markersize=gdf_final["cnt"], column="cnt", legend=True, alpha=0.6)
    ctx.add_basemap(ax, crs="epsg:3857", source=ctx.providers.Stamen.TonerLite, alpha=0.6)

    # Show/save
    if fig_location:
        plt.savefig(fig_location)
    if show_figure:
        plt.show()

if __name__ == "__main__":
    # zde muzete delat libovolne modifikace
    gdf = make_geo(pd.read_pickle("accidents.pkl.gz"))
    plot_geo(gdf, "geo1.png", True)
    plot_cluster(gdf, "geo2.png", True)

