# Codesplit Tutorial Examples

These libultra examples exist to demonstrate different approaches to segmenting large scale homebrew projects for the system. They are designed as companions for the Code Segmentation Guide on the N64Brew Wiki. It is therefore recommended that you read the Wiki pages in order to fully understand these samples.

### Sample Explanations
<ins>**original**</ins>  
**Description:** The original "bad" ROM, which all examples are based off.  
**Companion Page:** N/A

<ins>**splitdata-bin**</ins>  
**Description:** The original sample, but with the textures split from the codesegment and linked as binary files.  
**Companion Page:** https://n64brew.dev/wiki/Splitting_Assets_from_Code

<ins>**splitdata-binram**</ins>  
**Description:** The above sample, but using a buffer that is not part of the code segment.  
**Companion Page:** https://n64brew.dev/wiki/Splitting_Assets_from_Code#Having_a_buffer_somewhere_in_RAM

### Compiling
**Windows XP SDK:** Simply run `makeme.bat`.
