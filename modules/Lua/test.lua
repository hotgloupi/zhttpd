_:echo("<html><head><title>Faire une addition en lua</title></head><body>")
num1 = ""
num2 = ""
if _GET["num1"] and _GET["num2"] then
    num1 = _GET["num1"]
    num2 = _GET["num2"]
    _:echo("<p>")
    _:echo("Resultat = " .. _GET["num1"] + _GET["num2"])
    _:echo("</p>")
end
_:echo("<form method=\"get\"><p><input type=\"text\" name=\"num1\" value=\"" .. num1 .. "\"/> + <input type=\"text\" name=\"num2\" value=\"" .. num2 .. "\"/></p><p><input type=\"submit\" value=\"Ajouter\" /></p></form>")
_:echo("</body></html>")

-- if _GET["file"] then
--     f = io.open(_GET["file"])
--     if f then
--         _:echo("<pre>" .. f:read("*a") .. "</pre>")
--     else
--         _:echo("<p>file not found</p>")
--     end
-- end
_:echo("<p>chunk 1</p>")
-- _:flush()
--_:sleep(1)
_:echo("<p>chunk 2</p>")
-- _:flush()
--_:sleep(1)
_:echo("<p>chunk 3</p>")
-- _:flush()
--_:sleep(1)
_:echo("<p>chunk 4</p>")

_:echo('<form action="" method="post">')
_:echo('<p><input type="text" name="test" /></p>')
_:echo('<p><input type="submit" value="go" /></p>')
_:echo('</form>')

-- if _GET["file"] then
--     f = io.open(_GET["file"])
--     if f then
--         _:echo("<pre>" .. f:read("*a") .. "</pre>")
--     else
--         _:echo("<p>file not found</p>")
--     end
-- end

-- dofile("modules/Lua/test.lua")

