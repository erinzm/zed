(ns zed.repl
  (:require [zed.parser :as parser]
            [puget.printer :as puget]))

(defn repl []
  (do (print ">>> ")
      (flush))
  (let [input (read-line)]
    (puget/cprint (parser/parse-code input))
    (recur)))

(defn -main [& args]
  (repl))
