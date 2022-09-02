# Forensic-Watermarking-program-for-image-CRT
This document contains changes to the insert/extraction process in previous versions of the program (https://github.com/ms4935/Forensic-Watermarking-program-for-image).

# Development contents     
**In the current era of illegal copying and distribution of various image-based creative works, we aim to produce a forensic watermarking program that contributes to tracking by expanding the existing watermark application method.** 

* In the previous version, Forensic-Watermarking-program-for-image, during the insertion/extraction process, **Chinese rest theorem (CRT)** is applied to ensure security.

# [Original] Development enviroment

**Development Tools**: VisualStudio 2017

**Development Language**: C++

**Libraries used**: MFC, OpenCV

**Developers**: 4 (Team Project)

**Development Period**: July 2019 ~ October 2019

# [mrbsvf] Development enviroment

**Development Tools**: VisualStudio 2022

**Development Language**: C++

**Libraries used**: MFC, OpenCV

# Implement      
* **Chinese remainder theorem (CRT)**

* **Color Conversion**: Converts image color channels in RGB format to YUV color channels. Y is the image luminance, and U and V are the image chrominance signals. In JPEG compression, the data of the U and V channels, which the human eye is most insensitive to changes, is removed. Therefore, in the process of inverse conversion of the YUV color channel to RGB format in the last step, the Y channel, which is not removed during compression, is inserted, and JPEG is targeted for the Y channel to minimize the change due to lossy compression to the Y channel. Insertion proceeds according to the compression method.
    
    
![yuv](https://user-images.githubusercontent.com/13462458/76728631-a3dcc300-679a-11ea-944b-1e720331a0a1.PNG)
      
**QR code**
* Converts input information (string) into QR code using open source. In consideration of the JPEG compression method in the implementation of the embedding algorithm, downsampling of DWT, etc., it is converted to version 1-3 QR code so as not to exceed the watermark size of 32x32 size.

**Insert/Extract Algorithms**
* In general, the watermark embedding method is divided into spatial domain insertion and frequency domain insertion. In order to secure the project goals of invisibility and JPEG compression robustness, the implementation proceeds with the frequency domain insertion method.

* **Discrete wavelet transform(DWT)**: Binary wavelet transform is performed on the Y channel of the image obtained through color conversion to transform from the spatial domain to the frequency domain and insert. Through this, the image expressed by the pixel values ​​of the two-dimensional array is converted into an image expressed by the amount of change in the pixel values ​​of the two-dimensional array. That is, it is expressed as the amount of change in values ​​between adjacent pixels based on the position of a specific pixel. In addition, in the case of DWT, unlike general frequency conversion that includes only frequency (pixel value change) resolution, since it includes frequency resolution and resolution for the spatial domain (pixel value, corresponding location) to be converted, pixels at a specific location in the original image. The amount of change in values ​​can be expressed simultaneously.

![DWT](https://user-images.githubusercontent.com/13462458/74600613-57d52a80-50d7-11ea-9aa1-c079f1cd222b.PNG)

As a result of DWT, four subbands LL, LH, HL, and HH can be obtained.     

**The meaning of frequency in the image can be expressed as the amount of change in the pixel value. In the case of low frequencies, the amount of change in pixel values ​​is small, which is close to the background, and in the case of high frequencies, the amount of change in pixel values ​​is large and close to the boundary line.**
**Upper left** is the LL subband, which is equivalent to applying a low-pass filter in the horizontal and vertical directions to the original image.
(It is expressed as a high value because it does not go through the normalization process.)
**Top right** is the HL subband, which is equivalent to applying a high-pass filter in the vertical direction to the original image.
A vertical boundary line appears.
**Lower left** is the LH subband, which is equivalent to applying a high-pass filter in the horizontal direction to the original image.
A horizontal border line appears.
**Bottom right** is the HH subband, which is equivalent to applying a high-pass filter in the horizontal and vertical directions to the original image.
A diagonal boundary line appears.
**The change of low frequency representing the background has a great effect on the image quality and the human eye. On the other hand, since the change of high frequency that represents the boundary does not significantly affect the image quality and the human eye, select the HL, LH, and HH bands that include the high frequency region and proceed with insertion.**

* **Discrete cosine transform(DCT)**: DCT is performed once again on the image expressed as pixel value change through the previous DWT step. An image expressed as a pixel value change amount is an image indicating a boundary line of the corresponding image. A frequency band to be inserted can be selected in detail by performing DCT having only a frequency resolution on an image focused on the boundary line. ** That is, the edge included in the original image is expressed primarily through the DWT having resolution in the spatial and frequency domains, and the frequency included in the DWT result secondarily through the DCT having the resolution in the frequency domain Separately express the components.

![8x8](https://user-images.githubusercontent.com/13462458/75420337-3697f800-597b-11ea-9444-1daac46867dc.png)

In accordance with the JPEG compression method, the project proceeds with block DCT, which performs DCT on 8x8 matrix blocks for speed advantage.

![DCT](https://user-images.githubusercontent.com/13462458/75419072-5aa60a00-5978-11ea-880b-df220721796e.png)

DCT result has an expression range from low frequency component value **(DC value)** at position 0,0 to high frequency component value at position 7 and 7 **(AC value)**. As the position increases in the row and column directions from the value of the 0,0 position, the expressed frequency also changes from low frequency to high frequency. **JPEG compression reduces the amount of data and compresses the DCT result by changing the low frequency value that has a big impact on the human eye and changing the high frequency value with insignificant effect to a small amount. Therefore, the watermark pixel value is inserted into the DC value of the block DCT result for the original image.** Since a 32x32 watermark has 1024 pixel values, the watermark is added to each DC value of the 1024 block DCT result for the original image. Watermark insertion is completed by inserting mark pixel values.  
     
* **The extraction process is the reverse of the embedding process, and the watermark is restored by referring to the DC value through color conversion, DWT, and DCT.**

# Result
**Example results for Lena images**
     
![Lena_Result Screen](https://user-images.githubusercontent.com/13462458/76683089-9e6d6480-6644-11ea-9bef-5c0a05f34e7a.PNG)
     
         
**Table of quantitative figures for validation of results**

![ver1 result](https://user-images.githubusercontent.com/13462458/76498408-29513200-6480-11ea-99de-0c21b545d447.png)
     
     
* **PSNR (Maximum Signal-to-Noise Ratio)**: Used to evaluate image quality loss information in video or video lossy compression. The smaller the loss, the higher the value.

* **NCC (Normalized Correlation)**: Numerically represents the similarity between two images. (The closer to 1, the higher the similarity.)

**Demonstration video**
* Link to video demonstrating insertion/extraction program using MFC: https://youtu.be/cv6PpEdTOOw
* Link to demonstration video applying image download web: https://youtu.be/cDcwHMWT5zo

**Restrictions**
* **Restriction on the minimum size of the original image**: Because the standard size of the embedded watermark is set to 32x32, the size of the original image must satisfy at least 512 in both the horizontal and vertical size**.
A 32x32 watermark consists of a total of 1024 pixels. At this time, one watermark pixel is inserted into the 8x8 block of the block DCT result of the DWT result subband. Since a total of 32x32, that is, 1024 blocks of 8x8 blocks, which is the result of block DCT of the subband image, are needed, **subband image for DCT must satisfy the size of 256x256**. (8x32 = 512) Therefore, the size of 256x256 is The original image decomposed into 4 subbands must satisfy the size of 512x512.

* **3 subband insertion progress**: The algorithm proceeds to insert into all 3 subbands HL, LH, and HH among the 4 subbands of the DWT result for the original image. Each subband is represented by dividing a frequency included in the original image for each band. Inserting the watermark bit in all three subbands means inserting the watermark bit into 3/4 of the **frequency band representing the original image, and data changes occur a lot. The more the data changes, the more the inserted image differs from the original image, and the lower the image quality.** 

* **Lack of security**: Because it is blind watermarking that does not require the original image to extract the watermark from the embedded image,
The insertion method that simply changes the DC value according to the watermark pixel value can be easily removed by attackers.

* **Cannot extract from compressed images of a specific pattern**: In the case of an image with a specific pattern skewed to one side other than a general image with properly distributed low-frequency and high-frequency components, it cannot be extracted after JPEG compression. **In other words, JPEG compression robustness cannot be secured for images with specific patterns.**

**Future development tasks**
* **Solution of the minimum size limitation of the original image**: How to use a watermark smaller than 32x32 size by checking the configuration version of the QR code used as the watermark, **smaller than the 8x8 block DCT Consider using a block DCT**, inserting two watermark pixel values ​​(bits) into the result of an 8x8 block DCT.
    
* **Solution for inserting three subbands**: Perform image analysis on the original image and select the subband that contains the most values ​​in the original image representation among HL, LH, and HH subbands **Single insertion **Consider how to proceed.
(When inserting into a subband containing a small number of values ​​in the image representation, a change for a small value has a large effect on the result. However, when inserting into a subband containing a large number of values, a change for a large value There is no big difference from before, so it has a small effect on the result.)

* **Solution for lack of security**: A method of changing the DC value through an elaborate quantization step** in the step of inserting a watermark bit, a method of applying **Singular value decomposition (SVD)** , consider applying the **Chinese remainder theorem (CRT)**.
     
     
* **PSNR (Maximum Signal-to-Noise Ratio)**: Used to evaluate image quality loss information in video or video lossy compression. The smaller the loss, the higher the value.

* **NCC (Normalized Correlation)**: Numerically represents the similarity between two images. (The closer to 1, the higher the similarity.)

**Restrictions**
* **Restriction on the minimum size of the original image**: Because the standard size of the embedded watermark is set to 32x32, the size of the original image must satisfy at least 512 in both the horizontal and vertical size**.
A 32x32 watermark consists of a total of 1024 pixels. At this time, one watermark pixel is inserted into the 8x8 block of the block DCT result of the DWT result subband. Since a total of 32x32, that is, 1024 blocks of 8x8 blocks, which is the result of block DCT of the subband image, are needed, **subband image for DCT must satisfy the size of 256x256**. (8x32 = 512) Therefore, the size of 256x256 is The original image decomposed into 4 subbands must satisfy the size of 512x512.

* **3 subband insertion progress**: The algorithm proceeds to insert into all 3 subbands HL, LH, and HH among the 4 subbands of the DWT result for the original image. Each subband is represented by dividing a frequency included in the original image for each band. Inserting the watermark bit in all three subbands means inserting the watermark bit into 3/4 of the **frequency band representing the original image, and data changes occur a lot. The more the data changes, the more the inserted image differs from the original image, and the lower the image quality.**

* **Cannot extract from compressed images of a specific pattern**: In the case of an image with a specific pattern skewed to one side other than a general image in which low and high frequency components are properly distributed, it cannot be extracted after JPEG compression. **In other words, JPEG compression robustness cannot be secured for images with specific patterns.**

**Future development tasks**
* **Solution of the minimum size limitation of the original image**: How to use a watermark smaller than 32x32 size by checking the configuration version of the QR code used as the watermark, **smaller than the 8x8 block DCT Consider using a block DCT**, inserting two watermark pixel values ​​(bits) into the result of an 8x8 block DCT.
    
* **Solution for inserting three subbands**: Perform image analysis on the original image and select the subband that contains the most values ​​in the original image representation among HL, LH, and HH subbands **Single insertion **Consider how to proceed.
(When inserting into a subband containing a small number of values ​​in the image representation, a change for a small value has a large effect on the result. However, when inserting into a subband containing a large number of values, a change for a large value There is no big difference from before, so it has a small effect on the result.)

* **Cannot extract from compressed image of a specific pattern**: As with **3 subband insertion progress solution**, the low-frequency and high-frequency features of the image are analyzed through image analysis of the original image, and insertion into a specific pattern is performed. /Change the extraction method. In addition, the method is different according to the characteristics of the original image, such as adding weight to the AC value in addition to inserting the DC value.
