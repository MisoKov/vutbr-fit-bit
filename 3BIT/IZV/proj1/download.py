"""
Project: IZV projekt part 1
Author: Michal Koval <xkoval17@stud.fit.vutbr.cz>
"""

from bs4 import BeautifulSoup
import requests
import sys
import os.path
import zipfile
import csv
import io
import numpy as np
import gzip
import pickle


class IntIter:
    """
    Custom iterator class for integers
    """

    def __init__(self, list):
        """
        Initializer method
        :param list: list of string which should be mostly integers
        """
        self.list = list
        self.i = 0
        self.max_i = len(list)

    def __iter__(self):
        """
        Iterator method
        :return: returns self - iterator
        """
        return self

    def __next__(self):
        """
        Next method
        :return: if string is valid integer returns said integers, otherwise returns DataDownloader.int_invalid_value
        """
        if self.i >= self.max_i:
            raise StopIteration
        el = self.list[self.i]
        self.i += 1

        if el == "":
            return DataDownloader.int_invalid_value
        elif el.isdigit():
            return int(el)
        else:
            return DataDownloader.int_invalid_value


class TimeIter:
    """
    Custom iterator class for time - hours, minutes
    """

    def __init__(self, list):
        """
        Initializer method
        :param list: list of string which should be mostly integers, with semantics of time
        """
        self.list = list
        self.i = 0
        self.max_i = len(list)

    def __iter__(self):
        """
        Iterator method
        :return: returns self - iterator
        """
        return self

    def __next__(self):
        """
        Next method
        :return: int representing time, otherwise DataDownloader.int_invalid_value
        """
        if self.i >= self.max_i:
            raise StopIteration
        el = self.list[self.i]
        self.i += 1

        if el == "":
            return ""
        elif el == "2560":
            return DataDownloader.int_invalid_value
        elif el.isdigit():
            return int(el)
        else:
            return DataDownloader.int_invalid_value


class FloatIter:
    """
    Custom iterator class for floats
    """

    def __init__(self, list):
        """
        Initializer method
        :param list: list of string which should be mostly floats
        """
        self.list = list
        self.i = 0
        self.max_i = len(list)

    def __iter__(self):
        """
        Iterator method
        :return: returns self - iterator
        """
        return self

    def __next__(self):
        """
        Next method
        :return: if string is valid float returns said float, otherwise returns np.NaN
        """
        if self.i >= self.max_i:
            raise StopIteration
        el = self.list[self.i]
        self.i += 1

        if ',' in el:
            el = el.replace(',', '.')

        if el == "":
            return np.NaN
        else:
            try:
                f = float(el)
            except ValueError:
                f = np.NaN
            return f


class DataDownloader:
    """
    Class used to download and store data
    """

    """
    Constants used to determine the files corresponding to each region
    """
    PHA_FILE = ("00.csv", "PHA")
    STC_FILE = ("01.csv", "STC")
    JHC_FILE = ("02.csv", "JHC")
    PLK_FILE = ("03.csv", "PLK")
    KVK_FILE = ("19.csv", "KVK")
    ULK_FILE = ("04.csv", "ULK")
    LBK_FILE = ("18.csv", "LBK")
    HKK_FILE = ("05.csv", "HKK")
    PAK_FILE = ("17.csv", "PAK")
    OLK_FILE = ("14.csv", "OLK")
    MSK_FILE = ("07.csv", "MSK")
    JHM_FILE = ("06.csv", "JHM")
    ZLK_FILE = ("15.csv", "ZLK")
    VYS_FILE = ("16.csv", "VYS")

    col_names = ["region", "p1", "p36", "p37", "p2a", "weekday(p2a)", "p2b", "p6", "p7", "p8", "p9", "p10", "p11",
                 "p12", "p13a", "p13b", "p13c", "p14", "p15", "p16", "p17", "p18", "p19", "p20", "p21", "p22", "p23",
                 "p24", "p27", "p28", "p34", "p35", "p39", "p44", "p45a", "p47", "p48a", "p49", "p50a", "p50b", "p51",
                 "p52", "p53", "p55a", "p57", "p58", "a", "b", "d", "e", "f", "g", "h", "i", "j", "k", "l", "n", "o",
                 "p", "q", "r", "s", "t", "p5a"]

    int_invalid_value = -999999999

    def __init__(self, url="https://ehw.fit.vutbr.cz/izv/", folder="data", cache_filename="data_{}.pkl.gz"):
        """
        Constructor method
        :param url: Url of website where data are
        :param folder: Name of folder where data will be stored
        :param cache_filename: Template for naming of cache files
        """
        self.url = url
        self.folder = folder
        self.cache_filename = cache_filename
        self.pha_data = []
        self.stc_data = []
        self.jhc_data = []
        self.plk_data = []
        self.kvk_data = []
        self.ulk_data = []
        self.lbk_data = []
        self.hkk_data = []
        self.pak_data = []
        self.olk_data = []
        self.msk_data = []
        self.jhm_data = []
        self.zlk_data = []
        self.vys_data = []

    def download_data(self):
        """
        Downloads data from self.url to self.folder
        """
        # Fake header to prevent bot detection ( ಠ◡ಠ )
        header = {'User-Agent': 'Mozilla/5.0 (Windows NT 6.1; WOW64; rv:20.0) Gecko/20100101 Firefox/20.0'}
        # Getting the page content
        response = requests.get(self.url, headers=header)
        if response.status_code != 200:
            print("Error: Failed to connect to the url: " + self.url, file=sys.stderr)
        # Scrapping the data links
        soup = BeautifulSoup(response.text, "html.parser")
        zip_links = soup.find_all("a", class_="btn btn-sm btn-primary")
        # Checking if self.folder exists
        self.check_data_folder()
        # Choosing the most recent data for each year
        files = self.find_most_recent_data(zip_links)
        # Downloading data
        for filename in files:
            download_url = self.url + filename
            file = requests.get(download_url)
            f = open(os.path.join(self.folder, filename[5:]), "wb")
            f.write(file.content)
            f.close()

    def parse_region_data(self, region):
        """
        Parsed the data of one region from downloaded data files
        :param region: string containing name of ragion that should be parsed
        :return: parsed data
        """
        # Check data folder
        self.check_data_folder()
        # Check if data are downloaded
        if len(os.listdir(self.folder)) == 0:
            self.download_data()
        # Determining the csv file for region
        region_file = self.choose_region_file(region)
        # Reading data from files
        lines = []
        for zip_file in os.listdir(self.folder):
            if ".zip" in zip_file:
                with zipfile.ZipFile(self.folder + '/' + zip_file, "r") as zf:
                    with zf.open(region_file, "r") as csv_file:
                        csv_reader = csv.reader(io.TextIOWrapper(csv_file, encoding="windows-1250"), delimiter=';')
                        for line in csv_reader:
                            lines.append(np.array(line))

        # Transposing of array
        arr = np.transpose(lines)
        # Creating the final list of ndarrays of correct types
        element_count = len(arr[0])
        ret_list = []
        # Column of county code
        ret_list.append(np.full(element_count, region))
        # 3 columns of ints
        for i in range(3):
            ret_list.append(np.fromiter(IntIter(arr[i]), dtype=np.int_))
        # Date column
        ret_list.append(np.array(arr[3], dtype=np.datetime64))
        # Column of ints
        ret_list.append(np.fromiter(IntIter(arr[4]), dtype=np.int_))
        # Column of time
        ret_list.append(np.fromiter(TimeIter(arr[5]), dtype=np.int_))
        # 39 columns of ints
        for i in range(6, 45):
            ret_list.append(np.fromiter(IntIter(arr[i]), dtype=np.int_))
        # 6 columns of floats
        for i in range(45, 51):
            ret_list.append(np.fromiter(FloatIter(arr[i]), dtype=np.single))
        # 5 colums of strings
        for i in range(51, 56):
            ret_list.append(np.array(arr[i]))
        # Column of ints
        ret_list.append(np.fromiter(IntIter(arr[56]), dtype=np.int_))
        # 3 columns of string
        for i in range(57, 60):
            ret_list.append(np.array(arr[i]))
        # 2 column of ints
        for i in range(60, 62):
            ret_list.append(np.fromiter(IntIter(arr[i]), dtype=np.int_))
        # Column of strings
        ret_list.append(np.array(arr[62]))
        # Columns of ints
        ret_list.append(np.fromiter(IntIter(arr[63]), dtype=np.int_))

        return self.col_names, ret_list

    def get_list(self, regions=None):
        """
        Returns containing data of regions, utilizes memory and file caching
        :param regions: list of regions
        :return: data of regions in regions, if regions is None data of all regions will be returned
        """
        ret_list = []
        # Check if all regions
        if regions is None:
            regions = ["PHA", "STC", "JHC", "PLK", "KVK", "ULK", "LBK", "HKK", "PAK", "OLK", "MSK", "JHM", "ZLK", "VYS"]
        # Loading and joing data
        for region in regions:
            region_data = self.get_region_data(region)
            if not ret_list:
                ret_list = region_data
            else:
                for i in range(len(ret_list)):
                    ret_list[i] = np.concatenate((ret_list[i], region_data[i]))

        return self.col_names, ret_list

    def get_region_data(self, region):
        """
        Checks if the required data is cached and returns data from the best option
        :param region: name of region
        :return: parsed region data
        """
        # Check if cashed in memory
        if self.is_in_memory(region):
            return self.get_from_memory(region)
        # Check if cashed in file
        elif self.is_in_file_cache(region):
            data = self.load_data_from_file(region)
            self.save_data_to_memory(region, data)
            return data
        # Parsing is needed
        else:
            parsed_region = self.parse_region_data(region)
            self.save_data_to_memory(region, parsed_region[1])
            self.save_data_to_file(region, parsed_region[1])
            return parsed_region[1]

    def load_data_from_file(self, region):
        """
        Loads data from cache file
        :param region: region name
        :return: parsed region data
        """
        # Open file
        filename = self.region_cache_filename(region)
        with gzip.open(self.folder + "/" + filename, "rb") as f:
            # Deserialize with pickle
            return pickle.loads(f.read())

    def save_data_to_memory(self, region, data):
        """
        Saves parsed data to program memory
        :param region: name of region
        :param data: parsed data
        """
        if region == "PHA":
            self.pha_data = data
        elif region == "STC":
            self.stc_data = data
        elif region == "JHC":
            self.jhc_data = data
        elif region == "PLK":
            self.plk_data = data
        elif region == "KVK":
            self.kvk_data = data
        elif region == "ULK":
            self.ulk_data = data
        elif region == "LBK":
            self.lbk_data = data
        elif region == "HKK":
            self.hkk_data = data
        elif region == "PAK":
            self.pak_data = data
        elif region == "OLK":
            self.olk_data = data
        elif region == "MSK":
            self.msk_data = data
        elif region == "JHM":
            self.jhm_data = data
        elif region == "ZLK":
            self.zlk_data = data
        elif region == "VYS":
            self.vys_data = data
        else:
            raise Exception

    def save_data_to_file(self, region, data):
        """
        Save parsed data to file
        :param region: name of region
        :param data: parsed data
        """
        # Check if cache folder exists
        if not os.path.exists(self.folder):
            os.makedirs(self.folder)
        # Serialize with pickle
        ser_data = pickle.dumps(data)
        # Compress with gzip
        with gzip.open(self.folder + "/" + self.region_cache_filename(region), "wb") as f:
            f.write(ser_data)

    def region_cache_filename(self, region):
        """
        Generates cache filename for region according to template
        :param region: region name
        :return: cache file name
        """
        return self.cache_filename.replace("{}", region)

    def is_in_file_cache(self, region):
        """
        Checks if data are cached in file
        :param region: region name
        :return: True if cached, False otherwise
        """
        file_name = self.region_cache_filename(region)
        if os.path.exists(self.folder + "/" + file_name):
            return True
        else:
            return False

    def is_in_memory(self, region):
        """
        Checks if data are cached in memory
        :param region: region name
        :return: True if cached, False otherwise
        """
        if region == "PHA":
            return not self.pha_data == []
        elif region == "STC":
            return not self.stc_data == []
        elif region == "JHC":
            return not self.jhc_data == []
        elif region == "PLK":
            return not self.plk_data == []
        elif region == "KVK":
            return not self.kvk_data == []
        elif region == "ULK":
            return not self.ulk_data == []
        elif region == "LBK":
            return not self.lbk_data == []
        elif region == "HKK":
            return not self.hkk_data == []
        elif region == "PAK":
            return not self.pak_data == []
        elif region == "OLK":
            return not self.olk_data == []
        elif region == "MSK":
            return not self.msk_data == []
        elif region == "JHM":
            return not self.jhm_data == []
        elif region == "ZLK":
            return not self.zlk_data == []
        elif region == "VYS":
            return not self.vys_data == []
        else:
            raise Exception

    def get_from_memory(self, region):
        """
        Loads cached data from memory
        :param region: region name
        :return: parsed region data
        """
        if region == "PHA":
            return self.pha_data
        elif region == "STC":
            return self.stc_data
        elif region == "JHC":
            return self.jhc_data
        elif region == "PLK":
            return self.plk_data
        elif region == "KVK":
            return self.kvk_data
        elif region == "ULK":
            return self.ulk_data
        elif region == "LBK":
            return self.lbk_data
        elif region == "HKK":
            return self.hkk_data
        elif region == "PAK":
            return self.pak_data
        elif region == "OLK":
            return self.olk_data
        elif region == "MSK":
            return self.msk_data
        elif region == "JHM":
            return self.jhm_data
        elif region == "ZLK":
            return self.zlk_data
        elif region == "VYS":
            return self.vys_data
        else:
            raise Exception

    def choose_region_file(self, region):
        """
        Chooses region csv file name containing data
        :param region: region name
        :return: csv file name
        """
        if region == "PHA":
            file = self.PHA_FILE[0]
        elif region == "STC":
            file = self.STC_FILE[0]
        elif region == "JHC":
            file = self.JHC_FILE[0]
        elif region == "PLK":
            file = self.PLK_FILE[0]
        elif region == "KVK":
            file = self.KVK_FILE[0]
        elif region == "ULK":
            file = self.ULK_FILE[0]
        elif region == "LBK":
            file = self.LBK_FILE[0]
        elif region == "HKK":
            file = self.HKK_FILE[0]
        elif region == "PAK":
            file = self.PAK_FILE[0]
        elif region == "OLK":
            file = self.OLK_FILE[0]
        elif region == "MSK":
            file = self.MSK_FILE[0]
        elif region == "JHM":
            file = self.JHM_FILE[0]
        elif region == "ZLK":
            file = self.ZLK_FILE[0]
        elif region == "VYS":
            file = self.VYS_FILE[0]
        else:
            raise Exception
        return file

    def check_data_folder(self):
        """
        Checks if data folder exists, if not creates it
        """
        if os.path.exists(self.folder):
            if not os.path.isdir(self.folder):
                os.makedirs(self.folder)
        else:
            os.makedirs(self.folder)

    def find_most_recent_data(self, links):
        """
        Chooses the most recent data files for each year
        :param links: list of download links
        :return: list of most recent files links
        """
        ret_links = []
        file_temp = links[0]["href"]
        year_temp = file_temp[-8:-4]
        last_added = False
        for link in links:
            last_added = False
            file = link["href"]
            year = file[-8:-4]
            if year > year_temp:
                ret_links.append(file_temp)
                year_temp = year
                last_added = True
            file_temp = file
        if not last_added:
            ret_links.append(file_temp)
        return ret_links


if __name__ == "__main__":
    dd = DataDownloader()
    ret_list = dd.get_list(["JHM", "ZLK", "VYS"])
    print("Columns:")
    for col in ret_list[0]:
        print(col, end=" | ")
    print("\nNumber of entries: " + str(len(ret_list[1][0])))
    county_set = set(ret_list[1][0])
    print("Counties: ")
    for c in county_set:
        print(c, end=" ")
    print()
