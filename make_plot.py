# make_plot.py


import json
import sys
from scipy.signal import find_peaks
import matplotlib.pyplot as plt
import numpy as np


def import_name():
    """Extract the name of the desired test file using command line arguments
    When running the ecg_analysis.py program, the user is asked to provide
    the name of the desired test file in the command line. This function
    extracts the name of the file from the command line, returning this name
    as a string.
    Parameters
    ----------
    Returns
    -------
    string
        Name of the file
    """
    filename = sys.argv[1]
    return filename


def import_data(filename):
    """Import data from a given file name
    This function reads in a file name as a parameter and creates a list
    of strings. The strings are first separated by line, and then each string
    is stripped of its '\n' at the end of each string. This allows for
    better/ easier manipulation of the contents of the file.
    Parameters
    ----------
    filename : string
        Name of the desired test file
    Returns
    -------
    list
        List of strings, each one a different line from the file
    """
    contents = list()
    with open(filename, 'r') as input_file:
        file = input_file.readlines()
        for line in file:
            contents.append(line.strip("\n"))
        return contents


def str_to_float(contents):
    """Convert a list of strings into a list of floats
    To actually utilize the time and voltage data points, they both must
    be converted from strings to floats. In order to do this, this function
    reads in a list of two strings, one being the time as a string and the
    other being the voltage as a string. From here, it tries to convert each
    string into the type float. If it is not possible because the string
    contains letters or incorrect punctuation, then the function will log
    the following error: "This line of data is not usable: line skipped".
    It will add any data type (float or string) to the output list,
    so if the input list of two strings contains only one string that could
    be converted to a float, then it will return a list of size two containing
    the float value and a string.
    Parameters
    ----------
    input_results : list
        List containing two strings
    Returns
    -------
    list
        Contains two elements, where each could be a string or a float
    """
    data = list()
    for item in contents:
        data.append(float(item))
    return data


def finding_peaks(voltage):
    """Find the peaks for a given list
    Another of the calculated data was the number of detected beats in the
    strip. To determine this, the scipy package must be installed, and from
    this package import the package "find_peaks". This package is able to
    find the peaks of an incoming signal. This function utilizes this
    package to create a list of integers containing the indices of all
    of the peaks within the voltage list. The voltage list is inputted,
    then using this package, the length of the resulting list is found.
    This length is equivalent to the number of peaks present, so this
    value is returned.
    Parameters
    ----------
    voltage : list
        List of floats containing the voltage values
    Returns
    -------
    list
        List of all peaks
    """
    peaks, properties = find_peaks(voltage, height=None)
    return peaks, properties


def get_peaks(peaks, data):
    """ Get the peaks from the raw data

    Parameters
    ----------
    peaks: list
        List of indices for location of peaks
    data: list
        List of raw data from imported excel file
    Returns
    -------
    list
        List of values for peaks
    """
    print("Peaks: {}".format(peaks))
    peak_values = list()
    for value in peaks:
        peak_values.append(data[value])
    print(peak_values)
    return peak_values


def generate_indices(input_list):
    """ Generate a list for x axis of plots based on size of data

    Parameters
    ----------
    input_list: list
        List of raw data from imported excel file
    Returns
    -------
    list
        List for x axis of plots
    """
    num_values = len(input_list)
    x_axis = list(range(0, num_values))
    return x_axis


def generate_plots(length, data, title, plot_count):
    """ Generate plots for the data

    Parameters
    ----------
    length: list
        List for x axis of plots
    data: list
        List of data from imported excel file
    title: str
        Desired title for plot
    plot_count: int
        The number of the figure
    Returns
    -------
    N/A
        This function does not return a value
    """
    plt.figure(plot_count)
    plt.plot(length, data)
    plt.title(title)
    return 0


if __name__ == '__main__':
    filename = import_name()
    print(filename)
    file_contents = import_data(filename)
    raw_data = str_to_float(file_contents)
    [peaks, properties] = finding_peaks(raw_data)
    peak_values = get_peaks(peaks, raw_data)
    length = generate_indices(raw_data)
    generate_plots(length, raw_data, "Raw Data", 1)
    length_2 = generate_indices(peak_values)
    generate_plots(length_2, peak_values, "Edited for only max values", 2)
    plt.show()
