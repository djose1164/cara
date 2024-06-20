function getFixedImagesArr(arr) {
    return arr.map(item => {
        return { imageUrl: item.image_url, id: item.id }
    });
}
