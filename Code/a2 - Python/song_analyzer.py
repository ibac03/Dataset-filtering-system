#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Import necessary libraries: argparse for command-line parsing, pandas for data manipulation, and datetime for handling dates.
import argparse
import pandas as pd
from datetime import datetime

"""
Created on Wed Jan 8 14:44:33 2024
Based on: https://www.kaggle.com/datasets/nelgiriyewithana/top-spotify-songs-2023
Sample input: --filter="ARTIST" --value="Dua Lipa" --order_by="STREAMS" --order="ASC" --limit="6"'
@author: bharath 
@author: V01024896
"""

# Function to load data from a specified CSV file path. Returns a pandas DataFrame.
def fetch_dataset(filepath):
    # Use pandas to read the CSV file at the given filepath and return the DataFrame.
    return pd.read_csv(filepath)

# Applies a filter to the dataset based on a given criterion (artist or year).
def apply_filter(dataset, filter_key, filter_value):
    # Check if the filter is for the artist.
    if filter_key == "ARTIST":
        # Perform case-insensitive filtering by artist name using the str.contains method.
        filtered_data = dataset[dataset['artist(s)_name'].str.contains(filter_value, case=False, na=False)]
    # Check if the filter is for the year.
    elif filter_key == "YEAR":
        # Filter rows where the 'released_year' column matches the specified year.
        filtered_data = dataset[dataset['released_year'] == int(filter_value)]
    # Default case if filter_key is not recognized. No filtering is applied.
    else:
        filtered_data = dataset
    # Return the filtered dataset.
    return filtered_data

# Function to sort the dataset by a specific column and limit the number of results.
def order_and_limit_songs(dataset, sort_by, is_ascending=True, result_limit=None):
    # Sort the dataset by the specified column. 'ascending' determines the sort direction.
    ordered_data = dataset.sort_values(by=sort_by, ascending=is_ascending)
    # Apply a limit to the number of results if specified, else return all sorted data.
    return ordered_data.head(result_limit if result_limit is not None else len(dataset))

# Adds formatted release date strings to each song record in the dataset.
def enhance_release_date(record):
    try:
        # Create a datetime object from the year, month, and day columns.
        date_obj = datetime(record['released_year'], record['released_month'], record['released_day'])
        # Format the date as 'DayOfWeek, Month Day, Year'.
        return date_obj.strftime('%a, %B %d, %Y')
    except ValueError as error:
        # Print an error message if date formatting fails, usually due to invalid dates.
        print(f"Formatting error for record: {record}, error: {error}")
        # Return None to indicate failure.
        return None

# Prepares the final dataset for output, ensuring it has the correct columns and formatting.
def finalize_dataset(dataset, sort_criteria):
    # Format each record's release date using 'enhance_release_date' and store it in a new 'released' column.
    dataset['released'] = dataset.apply(enhance_release_date, axis=1)
    # Map sorting criteria to the actual column names in the DataFrame.
    column_selection = {
        'NO_SPOTIFY_PLAYLISTS': 'in_spotify_playlists',
        'NO_APPLE_PLAYLISTS': 'in_apple_playlists'
    }.get(sort_criteria, 'streams')  # Default to 'streams' if sort_criteria is not recognized.
    # Select only the relevant columns for the final output.
    final_data = dataset[['released', 'track_name', 'artist(s)_name', column_selection]]
    # Return the data, excluding any records with missing release dates.
    return final_data.dropna(subset=['released'])

# Conducts the full song analysis workflow, from data loading to saving the final output.
def conduct_analysis(parameters):
    # Load the dataset from the specified CSV file.
    dataset = fetch_dataset(parameters.data_file)
    # Apply user-specified filtering.
    dataset = apply_filter(dataset, parameters.filter, parameters.value)
    # Determine sort order: true for ascending, false for descending.
    is_ascending = parameters.order.upper() == "ASC"
    # Correctly map the 'order_by' argument to the actual DataFrame column names.
    sort_column_map = {
        'NO_SPOTIFY_PLAYLISTS': 'in_spotify_playlists',
        'NO_APPLE_PLAYLISTS': 'in_apple_playlists',
        'STREAMS': 'streams'
    }
    # Default to 'streams' if 'order_by' is not recognized.
    sort_by_corrected = sort_column_map.get(parameters.order_by.upper(), 'streams')
    # Sort the data and apply any specified result limit.
    dataset = order_and_limit_songs(dataset, sort_by_corrected, is_ascending, parameters.limit)
    # Prepare the final dataset for output.
    final_data = finalize_dataset(dataset, parameters.order_by.upper())
    # Save the final dataset to 'output.csv', as per user request.
    persist_to_csv(final_data, "output.csv")
    # Notify the user of where the results have been saved.
    print("Analysis results are saved in output.csv.")

# Saves the provided DataFrame to a CSV file, named according to 'file_name'.
def persist_to_csv(dataset, file_name):
    # Use pandas to write the DataFrame to a CSV file, without the index.
    dataset.to_csv(file_name, index=False)

# Entry point of the script, handling command-line arguments and triggering the analysis.
def main():
    # Set up command-line argument parsing.
    arg_parser = argparse.ArgumentParser(description="Data analysis on Spotify songs dataset.")
    # Define expected command-line arguments.
    arg_parser.add_argument('--data_file', type=str, required=True, help="Dataset file path.")
    arg_parser.add_argument('--filter', type=str, choices=['ARTIST', 'YEAR'], help="Filtering criteria.")
    arg_parser.add_argument('--value', type=str, help="Filter value.")
    arg_parser.add_argument('--order_by', type=str, required=True, choices=['STREAMS', 'NO_SPOTIFY_PLAYLISTS', 'NO_APPLE_PLAYLISTS'], help="Sorting criterion.")
    arg_parser.add_argument('--order', type=str, required=True, choices=['ASC', 'DES'], help="Sorting order.")
    arg_parser.add_argument('--limit', type=int, help="Maximum number of results to return.")
    # Parse command-line arguments.
    args = arg_parser.parse_args()
    # Perform the analysis based on the provided arguments.
    conduct_analysis(args)

# Ensure that the script can be run directly from the command line.
if __name__ == '__main__':
    main()