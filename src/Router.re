type route =
  | Featured
  | Subscription
  | Favorites
  | NotFound;

let toUrl = r => {
  let url =
    switch (r) {
    | Featured => "featured"
    | Subscription => "subscription"
    | Favorites => "favorites"
    | NotFound => "notfound"
    };

  "#/" ++ url;
};

let mapUrlToRoute = (url: React.Router.url): route =>
  switch (url.hash) {
  | ""
  | "/"
  | "/featured" => Featured
  | "/subscription" => Subscription
  | "/favorites" => Favorites
  | _ => NotFound
  };

let push = (r: route) => React.Router.push(r |> toUrl);

let watchUrl = React.Router.watchUrl;

let unwatchUrl = React.Router.unwatchUrl;

let currentRoute = () =>
  React.Router.dangerouslyGetInitialUrl() |> mapUrlToRoute;

let dangerouslyGetInitialUrl = React.Router.dangerouslyGetInitialUrl;