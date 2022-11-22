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
bash icns-creator/script.sh pendoctor.png
```
