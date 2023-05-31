@echo off

echo 削除します。宜しいですか？ 


del *.sdf /S /Q
del Debug /S /Q
rmdir Debug /S /Q
del Release /S /Q
rmdir Release /S /Q
del ipch /S /Q
rmdir ipch /S /Q
del xml_lib\Debug /S /Q
rmdir xml_lib\Debug /S /Q
del xml_lib\Release /S /Q
rmdir xml_lib\Release /S /Q
del AutoMouse\Debug /S /Q
rmdir AutoMouse\Debug /S /Q
del AutoMouse\Release /S /Q
rmdir AutoMouse\Release /S /Q


git add .

