# console-qrencode
Command line program to create QR code (PNG or SVG).

About
---
Contains an alternative to the "qrencode" distribtion from [libqrencode](http://fukuchi.org/works/qrencode/index.html.ja).

In particular:

* The SVG output contains "xmlns:ns4d" to dictate the DPI (4D proprietary attribute).

* PNG output can be sent to stdout on Windows.

**Note**: Because the default mode of stdout is text, LF is converted to CRLF on Windows. As a result, binary output to stdout is most probably damaged. To avoid this, ```_setmode``` is used.  [_setmode](https://msdn.microsoft.com/ja-jp/library/tw4k6df8.aspx)

* Unicode file names are accepted on Windows. ```_wfopen``` is used instead of ```fopen``` [_wfopen](https://msdn.microsoft.com/ja-jp/library/yeby3zcb.aspx)

* POSIX paths are accepted on Windows. (simple replacement of back slash by forward slash).
