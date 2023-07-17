# SPDX-FileCopyrightText: Copyright DB Netz AG
# SPDX-License-Identifier: CC0-1.0

FROM python:3.11-slim

# # Update package lists
# RUN apt-get update -y

# # Install curl
# RUN apt-get install -y curl

# # Set environment varibles
# ENV PYTHONDONTWRITEBYTECODE 1
# ENV PYTHONUNBUFFERED 1

# Set work directory
WORKDIR /app

# Install Flask
RUN pip install --upgrade pip
RUN pip install flask

# Copy current directory contents into the container at /app
COPY . /app

# Make port 8080 available to the world outside this container
EXPOSE 8080

# Run the application when the container launches
CMD ["python3", "dbs_map_mock_server.py"]
