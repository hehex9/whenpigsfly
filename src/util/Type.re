module StringCompare = {
  type t = string;
  let compare = Pervasives.compare;
};

module StringSet = Set.Make(StringCompare);