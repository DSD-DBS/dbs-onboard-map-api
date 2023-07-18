# SPDX-FileCopyrightText: Copyright DB Netz AG
# SPDX-License-Identifier: CC0-1.0
FROM python:3.11-slim
ARG PORT=8080
# Set work directory
WORKDIR /app

# Install Flask
RUN pip install --upgrade pip
RUN pip install flask

COPY . /app
EXPOSE $PORT

# Run the DBS map mock server when the container launches
ENV FLASK_APP="dbs_map_mock_server.py"
ENTRYPOINT ["flask", "run", "--host=0.0.0.0", "--port=${PORT}"]
