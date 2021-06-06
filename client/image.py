from PIL import Image


def loadImage(path):
    try:
        image = Image.open(path).convert('RGBA')
        return image
    except:
        print("Error: Invalid file.")


def saveImage(path, data, width, height):
    try:
        result = Image.frombytes('RGBA', (width, height), data)
        result.save(path)
    except:
        print("Error: Not able to save received image data.")
