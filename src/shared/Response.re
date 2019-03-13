module Shape = {
  type responseList('a) = {
    isEnd: bool,
    next: int,
    list: array('a),
  };

  type create = {
    msg: string,
    code: int,
  };

  type featured = {
    id: string,
    type_: int,
    author: string,
    targetId: string,
    title: string,
    description: string,
    cover: string,
    createdAt: int,
  };

  type wechat = {
    id: string,
    name: string,
    avatar: string,
    description: string,
    isSubscribed: bool,
  };

  type wechatSimple = {
    id: string,
    name: string,
    avatar: string,
    isSubscribed: bool,
  };

  type article = {
    id: string,
    title: string,
    cover: string,
    isFavorited: bool,
    createdAt: int,
    wechat: wechatSimple,
  };

  type articleDetail = {
    id: string,
    author: string,
    copyright: int,
    title: string,
    cover: string,
    content: string,
    isFavorited: bool,
    createdAt: int,
    wechat: wechatSimple,
  };

  type category = {
    id: string,
    name: string,
    description: string,
    cover: string,
    weight: int,
  };
};

module Decode = {
  open Shape;

  let featured = json: featured =>
    Json.Decode.{
      id: json |> field("id", string),
      type_: json |> field("type", int),
      author: json |> field("author", string),
      targetId: json |> field("target_id", string),
      title: json |> field("title", string),
      description: json |> field("description", string),
      cover: json |> field("cover", string),
      createdAt: json |> field("created_at", int),
    };

  let wechatSimple = json: wechatSimple =>
    Json.Decode.{
      id: json |> field("id", string),
      name: json |> field("name", string),
      avatar: json |> field("avatar", string),
      isSubscribed: json |> field("is_subscribed", bool),
    };

  let wechat = json: wechat =>
    Json.Decode.{
      id: json |> field("id", string),
      name: json |> field("name", string),
      avatar: json |> field("avatar", string),
      description: json |> field("description", string),
      isSubscribed: json |> field("is_subscribed", bool),
    };

  let article = json: article =>
    Json.Decode.{
      id: json |> field("id", string),
      title: json |> field("title", string),
      cover: json |> field("cover", string),
      isFavorited: json |> field("is_favorited", bool),
      createdAt: json |> field("created_at", int),
      wechat: json |> field("wechat", wechatSimple),
    };

  let articleDetail = json: articleDetail =>
    Json.Decode.{
      id: json |> field("id", string),
      title: json |> field("title", string),
      cover: json |> field("cover", string),
      author: json |> field("author", string),
      copyright: json |> field("copyright", int),
      content: json |> field("content", string),
      isFavorited: json |> field("is_favorited", bool),
      createdAt: json |> field("created_at", int),
      wechat: json |> field("wechat", wechatSimple),
    };

  let category = json: category =>
    Json.Decode.{
      id: json |> field("id", string),
      name: json |> field("name", string),
      description: json |> field("description", string),
      cover: json |> field("cover", string),
      weight: json |> field("weight", int),
    };

  let result = (json, decoder) =>
    Json.Decode.(json |> field("result", decoder));

  let list = (json, decoder) =>
    Json.Decode.{
      next: json |> field("next", int),
      list: json |> field("list", array(decoder)),
      isEnd: json |> field("is_end", bool),
    };

  let featuredList = json => list(json, featured);
  let articleList = json => list(json, article);
  let wechatList = json => list(json, wechat);
  let categoryList = json => list(json, category);

  let featuredListResult = json => result(json, featuredList);
  let articleListResult = json => result(json, articleList);
  let wechatListResult = json => result(json, wechatList);
  let categoryListResult = json => result(json, categoryList);
  let categoryResult = json => result(json, category);
  let articleContent = json => result(json, articleDetail);
  let wechatResult = json => result(json, wechat);

  let afterCreate = json =>
    Json.Decode.(
      json
      |> field("result", json =>
           {
             code: json |> field("code", int),
             msg: json |> field("msg", string),
           }
         )
    );
  let afterRemove = afterCreate;
};

module Encode = {
  let featured = (c: Shape.featured) =>
    Js.Json.(
      Json.Encode.object_([
        ("id", string(c.id)),
        ("author", string(c.author)),
        ("type", number(float_of_int(c.type_))),
        ("target_id", string(c.targetId)),
        ("title", string(c.title)),
        ("description", string(c.description)),
        ("cover", string(c.cover)),
        ("created_at", number(float_of_int(c.createdAt))),
      ])
    );

  let wechatSimple = (c: Shape.wechatSimple) =>
    Js.Json.(
      Json.Encode.object_([
        ("id", string(c.id)),
        ("name", string(c.name)),
        ("avatar", string(c.avatar)),
        ("is_subscribed", boolean(c.isSubscribed)),
      ])
    );

  let article = (c: Shape.article) =>
    Js.Json.(
      Json.Encode.object_([
        ("id", string(c.id)),
        ("title", string(c.title)),
        ("cover", string(c.cover)),
        ("is_favorited", boolean(c.isFavorited)),
        ("created_at", number(float_of_int(c.createdAt))),
        ("wechat", wechatSimple(c.wechat)),
      ])
    );

  let category = (c: Shape.category) =>
    Js.Json.(
      Json.Encode.object_([
        ("id", string(c.id)),
        ("name", string(c.name)),
        ("description", string(c.description)),
        ("cover", string(c.cover)),
        ("weight", number(float_of_int(c.weight))),
      ])
    );

  let _list = (c: Shape.responseList('a), encoder) =>
    Js.Json.(
      Json.Encode.object_([
        ("next", number(float_of_int(c.next))),
        ("list", Json.Encode.array(encoder, c.list)),
        ("is_end", boolean(c.isEnd)),
      ])
    );

  let articleList = c => _list(c, article);
  let featuredList = c => _list(c, featured);
  let categoryList = c => _list(c, category);
};