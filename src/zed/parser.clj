(ns zed.parser
  (:require [instaparse.core :as insta]))

(def parser
  "parse some Zed code"
  (insta/parser
   (clojure.java.io/resource "zed.ebnf")))

(defn transform-ast
  [ast]
  (insta/transform {:number (fn [number] [:number (read-string number)])
                    :expression identity} ; pourthrough for expressions
                   ast))

(defn parse-code
  [code]
  (->> code
       parser
       transform-ast))
