(ns zed.parser
  (:require [instaparse.core :as insta]))

(def parser
  "parse some Zed code"
  (insta/parser
   (clojure.java.io/resource "zed.ebnf")))

(defn transform-ast
  [ast] ast)

(defn parse-code
  [code]
  (->> code
       parser
       transform-ast))
