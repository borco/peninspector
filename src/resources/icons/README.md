# Resources

## macOS

### Generating .icns files

* clone [icns-creator](https://github.com/juhosa/icns-creator)
* image should be larger than 1024x1024 (or at least 512x512)

```bash
# install imagemagick
brew install imagemagick

# create catalog .icns
cd src/resources/icons
git clone https://github.com/juhosa/icns-creator

### convert SVG to 1024x1024 images
bash icns-creator/script.sh peninspector.png
```

## Windows

### Generating .ico files

* install `imagemagick`
* [reference](https://gist.github.com/pfig/1808188)

```bash
# install imagemagick (in a WSL console)
sudo apt install imagemagick

# open a WSL bash console
# cd to the location of src/resources/icons
convert peninspector.png -define icon:auto-resize=256,128,64,48,32,16 peninspector.ico
```
