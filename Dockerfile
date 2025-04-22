# Using an image for thirdparty, where all dependencies are gathered
FROM wxopenglpng-thirdparty

# Copy the entire project into the container
WORKDIR /wxopenglpng
COPY . .

# Create a directory for the build
RUN mkdir -p build

# Start building the project
RUN cmake --build build --parallel

# Set the default command (can be changed to the command to run the project)
CMD ["./build/subprojects/Build/wx_opengl_tutorial_core/main"]