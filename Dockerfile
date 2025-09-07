# syntax=docker/dockerfile:1.7

# Build stage
FROM ubuntu:22.04 AS build
RUN apt-get update && apt-get install -y g++ make libsqlite3-dev
WORKDIR /app
COPY . .
RUN make

# Runtime stage
FROM ubuntu:22.04
RUN apt-get update && apt-get install -y libsqlite3-0 curl
WORKDIR /app
COPY --from=build /app/credit-report-web /app/credit-report-web
EXPOSE 8080
HEALTHCHECK CMD curl -fsS http://localhost:8080/healthz || exit 1
ENTRYPOINT ["./credit-report-web"]
