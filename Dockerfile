FROM debian:latest
COPY . /
RUN apt update && apt install -y libncurses5-dev libncursesw5-dev libgmp-dev g++ && chmod +x /compile.sh && ./compile.sh
CMD ["/bin/imaginary"]